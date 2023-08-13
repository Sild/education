#[derive(Debug)]
pub struct Persons {
    pub person_id: i32,
    pub last_name: String,
    pub first_name: String,
    pub address: String,
    pub city: String,
}

#[derive(Debug)]
pub struct Cities {
    pub id: i32,
    pub name: String,
    pub size: i32,
}
