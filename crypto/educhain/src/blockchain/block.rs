use serde::{Deserialize, Serialize};
use std::fmt::{Display, Formatter};
use std::{fs};
use std::hash::{Hash, Hasher};
use std::io::Write;
use std::time::{SystemTime, UNIX_EPOCH};
use anyhow::{Result};


#[derive(Debug, Serialize, Deserialize, Hash, Clone, Eq, PartialEq)]
pub struct Block {
    pub index: u128,
    pub timestamp: u128,
    pub prev_hash: u64,
    pub data: Vec<u8>,
    pub nonce: u128,
    pub hash: u64,
}

impl Block {
    pub(crate) fn make_first<T>(hasher: &mut T) -> Self
    where
        T: Hasher,
    {
        let mut block = Self {
            index: 0,
            timestamp: get_cur_ts_ms(),
            prev_hash: 0,
            data: vec![0, 0, 0, 0, 0],
            nonce: 0,
            hash: 0,
        };
        block.hash(hasher);
        block.hash = hasher.finish();
        block
    }

    pub fn mine<T>(hasher: &mut T, prev_block: &Self, nonce: u128, data: &[u8]) -> Self
    where
        T: Hasher,
    {
        let mut block = Self {
            index: prev_block.index + 1,
            timestamp: get_cur_ts_ms(),
            prev_hash: prev_block.hash,
            data: data.to_owned(),
            nonce,
            hash: 0,
        };
        block.hash(hasher);
        block.hash = hasher.finish();
        block
    }

    pub fn load(file_path: &str) -> Option<Self> {
        let file = match fs::File::open(file_path) {
            Ok(file) => file,
            Err(e) => {
                log::error!("Fail to load file {}: {}", file_path, e);
                return None;
            }
        };
        let block: Self = match serde_json::from_reader(file) {
            Ok(block) => block,
            Err(e) => {
                log::error!("Failed to load block from file {}: {}", file_path, e);
                return None;
            }
        };
        Some(block)
    }

    pub fn dump(&self, data_dir: &str) -> Result<()> {
        let mut file = fs::File::create(gen_block_path(data_dir, self.index))?;
        file.write_all(serde_json::to_string(self)?.as_bytes())?;
        Ok(())
    }
}

fn gen_block_path(data_dir: &str, block_index: u128) -> String {
    format!("{}/{:0width$}.dat", data_dir, block_index, width = 64)
}

impl Display for Block {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}: {}->{}", self.index, self.prev_hash, self.hash)
    }
}

pub fn get_cur_ts_ms() -> u128 {
    let start = SystemTime::now();
    let since_the_epoch = start
        .duration_since(UNIX_EPOCH)
        .expect("Time went backwards");
    since_the_epoch.as_millis()
}

#[cfg(test)]
mod tests {
    use tempdir::TempDir;
    use crate::blockchain::Block;
    use crate::blockchain::block::gen_block_path;

    #[test]
    fn dump_load() -> anyhow::Result<()> {
        let tmp_dir = TempDir::new("chain_init")?;
        let data_path = tmp_dir.path().to_str().unwrap();
        assert!(std::fs::read_dir(data_path).unwrap().next().is_none());
        let block = Block {
            index: 1,
            timestamp: 2,
            prev_hash: 3,
            data: vec![4, 5, 6, 7, 8],
            nonce: 0,
            hash: 0,
        };
        block.dump(data_path)?;
        let loaded = Block::load(gen_block_path(data_path, block.index).as_str()).unwrap();
        assert_eq!(block, loaded);
        // no such block
        assert!(Block::load(gen_block_path(data_path, block.index + 1).as_str()).is_none());
        Ok(())
    }
}
