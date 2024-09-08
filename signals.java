public class Event {
    private Object object;
    private String id;

    public Event(Object object, Map<String, Object> attributes, String id) {
        this.object = object;
        this.id = id;
        this.attributes = new HashMap<>(attributes);
    }

    public Object getObject() {
        return object;
    }

    public String getId() {
        return id;
    }

    private Map<String, Object> attributes;

    public Map<String, Object> getAttributes() {
        return attributes;
    }
}

public class Clot {
    private Event event;
    private Object sender;
    private Callback callback;

    public Clot(Event event, Object sender, Callback callback) {
        this.event = event;
        this.sender = sender;
        this.callback = callback;
    }

    public void call(Object... args) {
        callback.call(event, args);
    }

    public interface Callback {
        void call(Event event, Object... args);
    }
}

public class Signal<T> {
    private List<Clot> subscriptions = new ArrayList<>();

    public void subscribe(Object obj) {
        subscriptions.add(new Clot(obj, obj, (event, args) -> ((Callback) obj).__clot__(event, args)));
    }

    public void unsubscribe(Object obj) {
        subscriptions.removeIf(clot -> clot.sender == obj);
    }

    public void publish(T data) {
        for (Clot clot : subscriptions) {
            clot.call(data);
        }
    }

    public List<Clot> getSubscriptions() {
        return subscriptions;
    }
}
