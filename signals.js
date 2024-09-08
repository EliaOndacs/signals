class Event {
  constructor(object, attributes, id = null) {
    Object.assign(this, attributes);
    this.object = object;
    this.id = id;
  }
}

class Clot {
  constructor(event, sender, callback) {
    this.callback = callback;
    this.event = event;
    this.sender = sender;
  }

  call(...args) {
    return this.callback(this.event, ...args);
  }
}

class Signal {
  constructor() {
    this._subscriptions = [];
  }

  subscribe(obj) {
    this._subscriptions.push(obj.__clot__());
  }

  unsubscribe(obj) {
    this._subscriptions = this._subscriptions.filter(
      (clot) => clot.sender !== obj
    );
  }

  publish(data) {
    this._subscriptions.forEach((clot) => clot.call(data));
  }

  get subscriptions() {
    return this._subscriptions;
  }
}
