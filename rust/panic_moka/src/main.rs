use std::future::Future;
use anyhow::{anyhow, Result};
use moka::future::Cache;


struct DBCache {
    storage: Cache<i32, Option<String>>,
}

impl DBCache {
    async fn get_or_init<F>(&self, key: i32, init: F) -> Result<Option<String>>
        where F: Future<Output = Result<Option<String>>> {

        let init_wrapper = async move {
            println!("init_wrapper");
            init.await
        };
        self.storage.try_get_with(key, init_wrapper).await.map_err(|e| anyhow!(e))
    }
}

#[tokio::main]
async fn main() {

    let db_cache = DBCache {
        storage: Cache::new(100500),
    };

    let init_some = async {
        println!("init async");
        Ok(Some("value".to_string()))
    };

    let val_some = db_cache.get_or_init(1, init_some).await.unwrap();

    let init_none = async {
        println!("init async");
        Ok(None)
    };

    let init_none_wrapper = async {
        println!("init_none_wrapper");
        init_none.await
    };

    let val_none = db_cache.get_or_init(2, init_none_wrapper).await.unwrap();

    println!("val_some: {:?}", val_some);
    println!("val_none: {:?}", val_none);

}
