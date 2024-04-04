use tonlib::client::TonClient;
use tonlib::client::TonClientInterface;
use tonlib::tl::{BlockId, BlocksShards};

async fn test() -> anyhow::Result<()> {
    let client = TonClient::builder().build().await?;
    let (_, info) = client.get_masterchain_info().await?;
    println!("MasterchainInfo: {:?}", &info);
    let block_id = BlockId {
        workchain: info.last.workchain,
        shard: info.last.shard,
        seqno: info.last.seqno,
    };
    let block_id_ext = client.lookup_block(1, &block_id, 0, 0).await?;
    println!("BlockIdExt: {:?}", &block_id_ext);
    let block_shards: BlocksShards = client.get_block_shards(&info.last).await?;
    // let mut shards = block_shards.shards.clone();
    println!("Shards: {:?}", &block_shards);
    Ok(())
}

#[tokio::main]
async fn main() {
    if test().await.is_err() {
        println!("Error")
    }
}
