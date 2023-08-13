mod objects;

use mysql_cdc::binlog_client::BinlogClient;
use mysql_cdc::binlog_options::BinlogOptions;
use mysql_cdc::errors::Error;
use mysql_cdc::events::binlog_event::BinlogEvent;
use mysql_cdc::replica_options::ReplicaOptions;
use mysql_cdc::ssl_mode::SslMode;

fn main() -> Result<(), Error> {
    // let options: BinlogOptions = BinlogOptions::from_start();
    let options: BinlogOptions = BinlogOptions::from_end();

    let options = ReplicaOptions {
        port: 32769,
        username: String::from("root"),
        password: String::from("password"),
        database: Some(String::from("test_db")),
        blocking: true,
        ssl_mode: SslMode::Disabled,
        binlog: options,
        ..Default::default()
    };

    let mut client = BinlogClient::new(options);

    for result in client.replicate()? {
        let (header, event) = result?;
        // println!("{:#?}", header);
        // println!("{:#?}", event);
        match event {
            BinlogEvent::WriteRowsEvent(_) => {
                println!("write event")
            }
            _ => {println!("ignore event")}
        }

        // You process an event here

        // After you processed the event, you need to update replication position

    }
    Ok(())
}