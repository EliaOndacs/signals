class Event {
  object: any;
  id: string | null;

  constructor(
    object: any,
    attributes: { [key: string]: any },
    id: string | null = null
  ) {
    Object.assign(this, attributes);
    this.object = object;
    this.id = id;
  }
}

class Clot {
  callback: (event: Event, ...args: any[]) => any;
  event: Event;
  sender: any;

  constructor(
    event: Event,
    sender: any,
    callback: (event: Event, ...args: any[]) => any
  ) {
    this.callback = callback;
    this.event = event;
    this.sender = sender;
  }

  call(...args: any[]): any {
    return this.callback(this.event, ...args);
  }
}

class Signal<T> {
  private _subscriptions: Clot[];

  constructor() {
    this._subscriptions = [];
  }

  subscribe(obj: { __clot__: () => Clot }) {
    this._subscriptions.push(obj.__clot__());
  }

  unsubscribe(obj: { __clot__: () => Clot }) {
    this._subscriptions = this._subscriptions.filter(
      (clot) => clot.sender !== obj
    );
  }

  publish(data: T) {
    this._subscriptions.forEach((clot) => clot.call(data));
  }

  get subscriptions(): Clot[] {
    return this._subscriptions;
  }
}
