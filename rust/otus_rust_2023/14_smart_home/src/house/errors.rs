use thiserror::Error;

#[derive(Error, Debug)]
pub enum HouseError {
    #[error("room name can't be empty")]
    EmptyRoomName(),
    #[error("room with name={0} not found")]
    RoomNotFound(String),
    #[error("room with name={0} already exists")]
    RoomAlreadyExists(String),
    #[error("can't remove non-empty room with name={0}")]
    NonEmptyRoomRemoving(String),
    #[error("device with id={0} already exists")]
    DeviceAlreadyExists(String),
    #[error("wrong device type={0}")]
    WrongDeviceType(String),
    #[error("device with id={0} not found")]
    DeviceNotFound(String),
}
