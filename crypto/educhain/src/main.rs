mod blockchain;

use anyhow::Result;
use async_std::task;
use async_std::task::JoinHandle;
use std::sync::{Arc, RwLock};
use std::time::Duration;
use tide::Request;

const CHAIN_DATA_PATH: &str = "/Users/sild/Projects/Personal/educhain/chain_data";

struct Context {
    chain: Arc<RwLock<blockchain::Chain>>,
}

#[async_std::main]
async fn main() -> Result<()> {
    env_logger::init();
    log::info!("App started");
    let chain = match blockchain::Chain::new(CHAIN_DATA_PATH) {
        Ok(chain) => Arc::new(RwLock::new(chain)),
        Err(err) => {
            log::error!("Failed to create chain: {}", err);
            return Err(err);
        }
    };
    let miner = miner(chain.clone());

    let ctx = Arc::new(Context {
        chain: chain.clone(),
    });

    let mut server = tide::with_state(ctx);
    server.at("/").get(|_:Request<_>| async move{ Ok("Hello") });
    server.at("/chain").get(chain_handler);
    server.listen("127.0.0.1:8080").await?;
    log::info!("Server is down");
    let _ = miner.await;
    log::info!("App is down");
    Ok(())
}

async fn chain_handler(req: Request<Arc<Context>>) -> tide::Result {
    let ctx = req.state();
    Ok(ctx.chain.read().unwrap().to_str().into())
}

fn miner(chain: Arc<RwLock<blockchain::Chain>>) -> JoinHandle<()> {
    task::spawn(async move {
        let mut iteration = 1;
        let mut nonce = 0;
        let mut hasher = chain.read().unwrap().get_hasher();
        loop {
            let last_block = chain.read().unwrap().get_last_block();
            let new_block =
                blockchain::Block::mine(&mut hasher, &last_block, nonce, &[iteration as u8]);
            if let Err(e) = chain.write().unwrap().add_block(new_block) {
                log::warn!(
                    "fail to add block on iteration={}, nonce={}: {}",
                    iteration,
                    nonce,
                    e
                );
                nonce += 1;
                continue;
            }
            nonce = 0;
            task::sleep(Duration::from_secs(5)).await;

            iteration += 1;
        }
    })
}
