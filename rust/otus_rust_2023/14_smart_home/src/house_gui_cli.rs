use iced::widget::{button, column, container, text, text_input};
use iced::{Element, Length, Sandbox, Settings};
use strum_macros::EnumIter;

// TODO wrong http url error isn't handled
fn get_state(url: &str) -> String {
    let req_url = url.to_string() + "?fmt=txt";
    let rsp: String = ureq::get(req_url.as_str())
        .call()
        .unwrap()
        .into_string()
        .unwrap();
    rsp
}

fn switch_status(url: &str, room_name: &str, device_id: &str) -> String {
    let req_url = url.to_string()
        + format!("/switch_status?room_name={room_name}&device_id={device_id}&fmt=txt").as_str();

    let rsp: String = match ureq::get(req_url.as_str()).call() {
        Ok(r) => r.into_string().unwrap(),
        Err(e) => match e.into_response() {
            Some(r) => r.into_string().unwrap(),
            None => "error during parsing the response".to_string(),
        },
    };
    rsp
}

#[derive(Debug, Clone, PartialEq, EnumIter)]
enum Message {
    SubmitUrl,
    ChangeUrl(String),
    SwitchSocketStatus,
    ChangeRoomId(String),
    ChangeSocketId(String),
    SubmitSwitch,
}

struct SmartHomeGUI {
    initialized: bool,
    url: String,
    switch_socket_status: bool,
    switch_room_id: String,
    switch_socket_id: String,
    state: String,
}

impl Sandbox for SmartHomeGUI {
    type Message = Message;
    fn new() -> Self {
        Self {
            initialized: false,
            url: String::from("http://localhost:8080"),
            switch_socket_status: false,
            switch_room_id: String::new(),
            switch_socket_id: String::new(),
            state: String::new(),
        }
    }

    fn title(&self) -> String {
        String::from("smart home gui cli (i hate gui)")
    }

    fn update(&mut self, message: Self::Message) {
        match message {
            Message::ChangeUrl(url) => self.url = url,
            Message::SubmitUrl => {
                self.initialized = true;
                self.state = get_state(self.url.as_str());
            }
            Message::SwitchSocketStatus => self.switch_socket_status = !self.switch_socket_status,
            Message::ChangeRoomId(room_id) => self.switch_room_id = room_id,
            Message::ChangeSocketId(socket_id) => self.switch_socket_id = socket_id,
            Message::SubmitSwitch => {
                self.state = switch_status(
                    self.url.as_str(),
                    self.switch_room_id.as_str(),
                    self.switch_socket_id.as_str(),
                );
                self.switch_socket_status = false;
            }
        }
    }

    fn view(&self) -> Element<Self::Message> {
        let mut content = column![].spacing(22);

        if !self.initialized {
            let ti = text_input("Enter smart_home url", self.url.as_str())
                .on_input(Message::ChangeUrl)
                .on_submit(Message::SubmitUrl);
            let bu = button("Submit").on_press(Message::SubmitUrl);
            content = content.push(ti).push(bu);
        } else {
            let report_txt = text(self.state.as_str()).size(20);
            let switch_socket_status_btn =
                button("Switch socket status").on_press(Message::SwitchSocketStatus);
            content = content.push(report_txt).push(switch_socket_status_btn);
            if self.switch_socket_status {
                let room_ti = text_input("enter room_id", self.switch_room_id.as_str())
                    .on_input(Message::ChangeRoomId);
                let device_id_ti = text_input("enter socket_id", self.switch_socket_id.as_str())
                    .on_input(Message::ChangeSocketId);
                let submit = button("Submit").on_press(Message::SubmitSwitch);
                content = content.push(room_ti).push(device_id_ti).push(submit);
            }
        }
        container(content)
            .width(Length::Fill)
            .height(Length::Fill)
            .center_x()
            .center_y()
            .into()
    }
}

fn main() -> iced::Result {
    SmartHomeGUI::run(Settings::default())
}
