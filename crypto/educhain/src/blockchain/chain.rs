use crate::blockchain::block::Block;
use anyhow::{anyhow, Result};
use std::hash::{DefaultHasher, Hasher};
use std::io::{Read};
use std::{fs, io};

pub struct Chain {
    data_dir: String,
    blocks: Vec<Block>,
    hasher: Box<dyn Hasher>,
}

unsafe impl Send for Chain {}
unsafe impl Sync for Chain {}

impl Chain {
    pub fn new(data_dir: &str) -> Result<Self> {
        let mut chain = Self {
            data_dir: data_dir.to_string(),
            blocks: vec![],
            hasher: create_hasher(),
        };
        match need_load(chain.data_dir.as_str()) {
            Ok(load) => match load {
                true => chain.load()?,
                false => chain.block_create_first()?,
            },
            Err(err) => return Err(err),
        }
        Ok(chain)
    }

    pub fn get_last_block(&self) -> Block {
        self.blocks.last().unwrap().clone()
    }

    fn load(&mut self) -> Result<()> {
        log::info!("Loading blockchain");
        let mut block_files: Vec<_> = fs::read_dir(&self.data_dir)?.map(|f| f.unwrap()).collect();
        block_files.sort_by_key(|f| f.path());
        self.blocks.clear();
        for block_file in block_files {
            let block_path = block_file.path().to_str().unwrap().to_string();
            log::debug!("Loading block from {}", block_path);
            let block = Block::load(block_path.as_str());
            if let Some(block) = block {
                if let Err(e) = self.block_is_valid(&block) {
                    return Err(anyhow!("Loaded block is not valid: {}, path={}: {}", block.hash, block_path, e));
                }
                self.blocks.push(block);
            }
        }
        Ok(())
    }

    pub fn add_block(&mut self, block: Block) -> Result<()> {
        self.block_is_valid(&block)?;
        log::debug!("hash={} added to the chain", block.hash);
        self.blocks.push(block);
        self.blocks.last().unwrap().dump(self.data_dir.as_str())
    }

    pub fn to_str(&self) -> String {
        let mut result = String::new();
        for block in self.blocks.iter() {
            result += format!("{};\n", block).as_str();
        }
        result
    }

    pub fn get_hasher(&self) -> Box<dyn Hasher + Send + Sync> {
        create_hasher()
    }

    #[allow(dead_code)]
    pub fn size(&self) -> usize {
        return self.blocks.len();
    }

    fn block_is_valid(&self, block: &Block) -> Result<()> {
        match self.blocks.last() {
            Some(prev_block) => {
                if block.index != prev_block.index + 1 {
                    return Err(anyhow!("Invalid block index: {}, prev_block_index: {}", block.index, prev_block.index))
                }
                if block.prev_hash != prev_block.hash {
                    return Err(anyhow!("Invalid block prev_hash: {}", block.index))
                }
            }
            None => {
                if block.index != 0 {
                    return Err(anyhow!("Invalid initial index: {}", block.index))
                }
                return Ok(())
            }
        }
        if block.hash > u64::MAX / 2 {
            return Err(anyhow!("Block hash is too big: {} > {}", block.hash, u64::MAX / 2));
        }
        Ok(())
    }

    fn block_create_first(&mut self) -> Result<()> {
        log::info!("creating first block");
        assert!(self.blocks.is_empty());
        let block = Block::make_first(&mut self.hasher);
        self.blocks.push(block);
        self.blocks.last().unwrap().dump(self.data_dir.as_str())?;
        Ok(())
    }
}

fn need_load(data_dir: &str) -> Result<bool> {
    fs::create_dir_all(data_dir)?;
    if fs::read_dir(data_dir)?.next().is_none() {
        return Ok(false);
    }
    println!("Chain data dir isn't empty.\nShould I Reset it or Load existing data? (R/L):");
    loop {
        let mut input = [0];
        let _ = io::stdin().read(&mut input);
        match input[0] as char {
            'r' | 'R' => {
                // cleanup everything
                fs::remove_dir_all(data_dir)?;
                fs::create_dir_all(data_dir)?;
                return Ok(false);
            }
            'l' | 'L' => {
                return Ok(true);
            }
            _ => println!("'Y' or 'n' only please."),
        }
    }
}

fn create_hasher() -> Box<dyn Hasher + Send + Sync> {
    Box::<DefaultHasher>::default()
}

#[cfg(test)]
mod tests {
    use tempdir::TempDir;
    use super::*;
    #[test]
    fn chain_init() -> Result<()>{
        let tmp_dir = TempDir::new("chain_init")?;
        let data_path = tmp_dir.path().to_str().unwrap();
        assert!(fs::read_dir(data_path).unwrap().next().is_none());
        let chain = Chain::new(data_path)?;
        let last_block = chain.get_last_block();
        assert_eq!(last_block.index, 0);
        assert_eq!(chain.size(), 1);
        println!("{}", data_path);
        assert!(fs::read_dir(data_path).unwrap().next().is_some());
        Ok(())
    }
}
