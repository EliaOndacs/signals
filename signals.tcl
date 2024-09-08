oo::class create Event {
    variable object id attributes

    constructor {object_ attributes_ id_} {
        set object $object_
        set id $id_
        set attributes $attributes_
    }
}

oo::class create Clot {
    variable event sender callback

    constructor {event_ sender_ callback_} {
        set event $event_
        set sender $sender_
        set callback $callback_
    }

    method call {data} {
        {*}$callback $event $data
    }
}

oo::class create Signal {
    variable subscriptions

    constructor {} {
        set subscriptions {}
    }

    method subscribe {obj callback} {
        set event [Event new $obj {} {}]
        set clot [Clot new $event $obj $callback]
        lappend subscriptions $clot
    }

    method unsubscribe {obj} {
        set subscriptions [lsearch -all -inline -not -exact [list $obj] $subscriptions]
    }

    method publish {data} {
        foreach clot $subscriptions {
            $clot call $data
        }
    }

    method getSubscriptions {} {
        return $subscriptions
    }
}


