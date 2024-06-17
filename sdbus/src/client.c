#include "client.h"

#include "common.h"

#include <systemd/sd-bus.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int handle_counter_changed(sd_bus_message *msg, void *userdata, sd_bus_error *ret_error)
{
    uint32_t value = 0;
    int ret = sd_bus_message_read_basic(msg, 'u', &value);
    if (ret < 0) {
        printf("read message error: %s\n", strerror(-ret));
        return ret;
    }
    printf("Count changed: %u\n", value);
    return ret;
}

int handle_increment(sd_bus_message *msg, void *userdata, sd_bus_error *ret_error)
{
    uint32_t value = 0;
    int ret = sd_bus_message_read_basic(msg, 'u', &value);
    if (ret < 0) {
        printf("read message error: %s\n", strerror(-ret));
        return ret;
    }
    printf("increment success, retrun value: %u\n", value);
    return ret;
}

void call_method()
{
    _cleanup_(sd_bus_unrefp) sd_bus *sdbus = NULL;
    _cleanup_(sd_bus_message_unrefp) sd_bus_message *message = NULL;
    _cleanup_(sd_bus_error_free) sd_bus_error error = SD_BUS_ERROR_NULL;
    int ret = sd_bus_default_user(&sdbus);
    if (ret < 0) {
        printf("sd_bus_default_user error: %s\n", strerror(-ret));
        return;
    }
    ret = sd_bus_call_method(sdbus,
                             DBUS_NAME,
                             DBUS_PATH,
                             DBUS_INTERFACE,
                             "IncrementBy",
                             &error,
                             &message,
                             "u",
                             1);
    if (ret < 0) {
        printf("sd_bus_call_method error: %s\n  bus error: %s\n", strerror(-ret), error.message);
        return;
    }
    int value = 0;
    ret = sd_bus_message_read(message, "u", &value);
    if (ret < 0) {
        printf("sd_bus_message_read error: %s\n", strerror(-ret));
        return;
    }
    if (value == 0) {
        printf("value is null\n");
        return;
    }
    printf("call method success, return value: %u\n", value);
}

void call_method_async()
{
    _cleanup_(sd_bus_unrefp) sd_bus *sdbus = NULL;
    _cleanup_(sd_bus_error_free) sd_bus_error error = SD_BUS_ERROR_NULL;
    int ret = sd_bus_default_user(&sdbus);
    if (ret < 0) {
        printf("sd_bus_default_user error: %s\n", strerror(-ret));
        return;
    }
    ret = sd_bus_call_method_async(sdbus,
                                   NULL,
                                   DBUS_NAME,
                                   DBUS_PATH,
                                   DBUS_INTERFACE,
                                   "IncrementBy",
                                   handle_increment,
                                   NULL,
                                   "u",
                                   1);
    if (ret < 0) {
        printf("sd_bus_call_method_async error: %s\n", strerror(-ret));
        return;
    }
    printf("call method async success\n");
}

int client_start(short mode)
{
    _cleanup_(sd_bus_message_unrefp) sd_bus_message *message = NULL;
    _cleanup_(sd_event_unrefp) sd_event *event = NULL;
    _cleanup_(sd_bus_unrefp) sd_bus *sdbus = NULL;
    int ret = 0;

    if ((ret = sd_event_default(&event)) < 0)
        return ret;
    if ((ret = sd_bus_default_user(&sdbus)) < 0)
        return ret;

    // set timeout
    /*uint64_t timeout = 0;*/
    /*if ((ret = sd_bus_get_method_call_timeout(sdbus, &timeout)) < 0)*/
    /*    return ret;*/
    /*printf("default timeout: %lu\n", timeout);*/
    /*if ((ret = sd_bus_set_method_call_timeout(sdbus, 2 * 1000 * 1000)) < 0)*/
    /*    return ret;*/

    if (mode == 0)
        call_method();
    if (mode == 1) {
        ret = sd_bus_match_signal(sdbus,
                                  NULL,
                                  DBUS_NAME,
                                  DBUS_PATH,
                                  DBUS_INTERFACE,
                                  "CountChanged",
                                  handle_counter_changed,
                                  NULL);
        if (ret < 0)
            return ret;
        call_method();
    }
    if (mode == 2) {
        call_method_async();
    }
    if ((ret = sd_bus_attach_event(sdbus, event, 0)) < 0)
        return ret;

    return sd_event_loop(event);
}
