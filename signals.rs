pub struct Event {
    object: Box<dyn std::any::Any>,
    id: Option<String>,
    attributes: std::collections::HashMap<String, Box<dyn std::any::Any>>,
}

impl Event {
    pub fn new(object: Box<dyn std::any::Any>, attributes: std::collections::HashMap<String, Box<dyn std::any::Any>>, id: Option<String>) -> Self {
        Event { object, id, attributes }
    }
}

pub struct Clot {
    event: Event,
    sender: Box<dyn std::any::Any>,
    callback: Box<dyn Fn(Event, Box<dyn std::any::Any>)>,
}

impl Clot {
    pub fn new(event: Event, sender: Box<dyn std::any::Any>, callback: Box<dyn Fn(Event, Box<dyn std::any::Any>)>) -> Self {
        Clot { event, sender, callback }
    }

    pub fn call(&self, data: Box<dyn std::any::Any>) {
        (self.callback)(&self.event, data);
    }
}

pub struct Signal {
    subscriptions: Vec<Clot>,
}

impl Signal {
    pub fn new() -> Self {
        Signal { subscriptions: Vec::new() }
    }

    pub fn subscribe(&mut self, obj: Box<dyn std::any::Any>, callback: Box<dyn Fn(Event, Box<dyn std::any::Any>)>) {
        let event = Event::new(obj.clone(), std::collections::HashMap::new(), None);
        let clot = Clot::new(event, obj, callback);
        self.subscriptions.push(clot);
    }

    pub fn unsubscribe(&mut self, obj: &Box<dyn std::any::Any>) {
        self.subscriptions.retain(|clot| clot.sender.as_ref() != obj);
    }

    pub fn publish(&self, data: Box<dyn std::any::Any>) {
        for clot in &self.subscriptions {
            clot.call(data.clone());
        }
    }

    pub fn get_subscriptions(&self) -> &Vec<Clot> {
        &self.subscriptions
    }
}
