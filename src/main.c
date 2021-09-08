/** Includes
*/
#include <stdio.h> // asprintf, printf
#include <stdlib.h> // malloc, free, exit, EXIT_FAILURE
#include <emscripten/websocket.h>

/** Dependencies
*/
#include "../dependencies/dcw/dcw.h"

/** Functions from dependencies
*/
extern void display_html(html_t raw_html);

/** Global variables
*/
EMSCRIPTEN_WEBSOCKET_T ws = (EMSCRIPTEN_WEBSOCKET_T)NULL;
char **clobs;
char *curr_clob;
int num_clobs;

/** Submodules
*/
#include "displays.c"
#include "utils.c"
#include "clobs.c"
#include "wss.c" // uses get_all_clobs, and split

/** Main function & display
*/
html_t main_h =
#include "../frontend/main.cml"
    ;
int main()
{
    display_html(main_h);

    if (!emscripten_websocket_is_supported())
    {
        printf("WebSockets are not supported, cannot continue!\n");
        exit(EXIT_FAILURE);
    }

    EmscriptenWebSocketCreateAttributes attr;
    emscripten_websocket_init_create_attributes(&attr);

    int isProd = EM_ASM_INT(
        { return window.location.hostname == "127.0.0.1" ? 1 : 0; });

    attr.url = isProd == 1 ? "ws://localhost:3000/" : "wss://clober-api.herokuapp.com";

    ws = emscripten_websocket_new(&attr);

    if (ws <= 0)
    {
        printf("WebSocket creation failed, error code %d!\n", (EMSCRIPTEN_RESULT)ws);
        exit(EXIT_FAILURE);
    }

    emscripten_websocket_set_onopen_callback(ws, (void *)42, wss_on_open);
    emscripten_websocket_set_onclose_callback(ws, (void *)43, wss_on_close);
    emscripten_websocket_set_onerror_callback(ws, (void *)44, wss_on_error);
    emscripten_websocket_set_onmessage_callback(ws, (void *)45, wss_on_message);
}