
#include <stdio.h>
#include <string.h>
#include "spotify/api.h"

typedef struct {
    void *userdata;
    char username[1024];
    char password[1024];
    sp_session_config config;
} mocking_data;

mocking_data g_data;

typedef enum event_type {
    MOCK_LOGGED_IN                   = 0,
    MOCK_LOGGED_OUT                  = 1,
    MOCK_METADATA_UPDATED            = 2,
    MOCK_CONNECTION_ERROR            = 3,
//    message_to_user             = 4,
//    notify_main_thread          = 5,
//    music_delivery              = 6,
//    play_token_lost             = 7,
//    log_message                 = 8
} event_type;


event_type eventq[16];
int events = 0;

void * sp_session_userdata(sp_session *session) {
    return g_data.config.userdata;
}

void sp_session_process_events(sp_session *session, int *next_timeout) {
    fprintf(stderr, "MOCK: sp_session_process_events called\n");
    fprintf(stderr, "MOCK: %d pending events\n", events);
    if(events > 0) {
        events--;
        event_type next = eventq[events];
        if(next == MOCK_LOGGED_IN) {
            fprintf(stderr, "MOCK: processing login event\n");
            g_data.config.callbacks->logged_in(session, SP_ERROR_OK);
        }
    }
    *next_timeout = 1;
    fprintf(stderr, "MOCK: sp_session_process_events completed\n");
}

const char * sp_user_canonical_name(sp_user *user) {
    fprintf(stderr, "MOCK: sp_user_canonical_name called\n");
    return g_data.username;
}

const char * sp_user_display_name(sp_user *user) {
    return "Mock display name";
}

bool sp_user_is_loaded(sp_user *user) {
    return 0;
}

sp_error sp_session_init(const sp_session_config *config, sp_session **sess) {
    fprintf(stderr, "MOCK: sp_session_init called\n");
    if(strcmp(config->application_key, "appkey_good"))
        return SP_ERROR_BAD_APPLICATION_KEY;
    fprintf(stderr, "MOCK: sp_session_init mark 1\n");
    g_data.config.cache_location = malloc(strlen(config->cache_location) + 1);
    g_data.config.settings_location = malloc(strlen(config->settings_location) + 1);
    g_data.config.application_key = malloc(config->application_key_size);
    g_data.config.user_agent = malloc(strlen(config->user_agent) + 1);
    g_data.config.callbacks = (sp_session_callbacks *)malloc(sizeof(sp_session_callbacks));
    g_data.config.userdata = config->userdata;

    fprintf(stderr, "MOCK: sp_session_init mark 2\n");
    g_data.config.api_version = config->api_version;
    fprintf(stderr, "MOCK: sp_session_init mark 3\n");
    strcpy((char *)g_data.config.cache_location, config->cache_location);
    fprintf(stderr, "MOCK: sp_session_init mark 4\n");
    strcpy((char *)g_data.config.settings_location, config->settings_location);
    fprintf(stderr, "MOCK: sp_session_init mark 5\n");
    memcpy((char *)g_data.config.application_key, config->application_key, config->application_key_size);
    fprintf(stderr, "MOCK: sp_session_init mark 6\n");
    strcpy((char *)g_data.config.user_agent, config->user_agent);
    fprintf(stderr, "MOCK: sp_session_init mark 7\n");
    memcpy((char *)g_data.config.callbacks, config->callbacks, sizeof(sp_session_callbacks));
    fprintf(stderr, "MOCK: sp_session_init mark 8\n");
    g_data.config.userdata = config->userdata;
    fprintf(stderr, "MOCK: sp_session_init completed\n");
    return SP_ERROR_OK;
}

sp_error sp_session_login(sp_session *session, const char *username, const char *password) {
    fprintf(stderr, "MOCK: sp_session_login called\n");
    strcpy(g_data.username, username);
    strcpy(g_data.password, password);
    eventq[events++] = MOCK_LOGGED_IN;
    fprintf(stderr, "MOCK: sp_session_login completed\n");
    return SP_ERROR_OK;
}

sp_error sp_session_logout(sp_session *session) {
    fprintf(stderr, "MOCK: sp_session_logout called\n");
    eventq[events++] = MOCK_LOGGED_OUT;
    fprintf(stderr, "MOCK: sp_session_logout completed\n");
    return SP_ERROR_OK;
}

const char* sp_error_message(sp_error error) {
    const char buff[1024];
    sprintf(buff, "Error number %d", error);
    return buff;
}

sp_user * sp_session_user(sp_session *session) {
    return (sp_user *)-1;
}

