#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Event {
    void* object;
    char* id;
    void* attributes;
} Event;

typedef struct Clot {
    Event* event;
    void* sender;
    void (*callback)(Event*, void*);
} Clot;

typedef struct {
    Clot** subscriptions;
    int subscription_count;
} Signal;

Event* event_new(void* object, void* attributes, char* id) {
    Event* event = malloc(sizeof(Event));
    event->object = object;
    event->id = id;
    event->attributes = attributes;
    return event;
}

Clot* clot_new(Event* event, void* sender, void (*callback)(Event*, void*)) {
    Clot* clot = malloc(sizeof(Clot));
    clot->event = event;
    clot->sender = sender;
    clot->callback = callback;
    return clot;
}

void clot_call(Clot* clot, void* data) {
    clot->callback(clot->event, data);
}

Signal* signal_new() {
    Signal* signal = malloc(sizeof(Signal));
    signal->subscriptions = NULL;
    signal->subscription_count = 0;
    return signal;
}

void signal_subscribe(Signal* signal, void* obj, void (*callback)(Event*, void*)) {
    Clot* clot = clot_new(NULL, obj, callback);
    signal->subscriptions = realloc(signal->subscriptions, (signal->subscription_count + 1) * sizeof(Clot*));
    signal->subscriptions[signal->subscription_count++] = clot;
}

void signal_unsubscribe(Signal* signal, void* obj) {
    for (int i = 0; i < signal->subscription_count; i++) {
        if (signal->subscriptions[i]->sender == obj) {
            free(signal->subscriptions[i]);
            signal->subscriptions[i] = signal->subscriptions[--signal->subscription_count];
            break;
        }
    }
}

void signal_publish(Signal* signal, void* data) {
    for (int i = 0; i < signal->subscription_count; i++) {
        clot_call(signal->subscriptions[i], data);
    }
}

Clot** signal_getSubscriptions(Signal* signal) {
    return signal->subscriptions;
}

// Example usage:
void callback(Event* event, void* data) {
    printf("Received data: %s\n", (char*)data);
}

int main() {
    Signal* signal = signal_new();
    signal_subscribe(signal, NULL, callback);
    signal_publish(signal, "Hello, world!");
    return 0;
}
