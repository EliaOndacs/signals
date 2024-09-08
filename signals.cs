public class Event {
    public object Object { get; }
    public string Id { get; }

    public Event(object @object, Dictionary<string, object> attributes, string id = null) {
        Object = @object;
        Id = id;
        Attributes = new Dictionary<string, object>(attributes);
    }

    public Dictionary<string, object> Attributes { get; }
}

public class Clot {
    public Event Event { get; }
    public object Sender { get; }
    public Callback Callback { get; }

    public Clot(Event @event, object sender, Callback callback) {
        Event = @event;
        Sender = sender;
        Callback = callback;
    }

    public void Call(params object[] args) {
        Callback(Event, args);
    }

    public delegate void Callback(Event @event, params object[] args);
}

public class Signal<T> {
    private List<Clot> subscriptions = new List<Clot>();

    public void Subscribe(object obj) {
        subscriptions.Add(new Clot(obj, obj, (event, args) => ((Callback)obj).__clot__(event, args)));
    }

    public void Unsubscribe(object obj) {
        subscriptions.RemoveAll(clot => clot.Sender == obj);
    }

    public void Publish(T data) {
        foreach (var clot in subscriptions) {
            clot.Call(data);
        }
    }

    public List<Clot> Subscriptions => subscriptions;
}
