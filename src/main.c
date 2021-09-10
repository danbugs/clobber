/** Includes
*/
#include <stdio.h>   // asprintf, printf
#include <stdlib.h>  // malloc, free, exit, EXIT_FAILURE
#include <stdbool.h> // bool, true, false
#include <emscripten/websocket.h>
#include <emscripten/threading.h>

/** Dependencies
*/
#include "../dependencies/dcw/dcw.h"

/** Functions from dependencies
*/
extern void display_html(html_t raw_html);
extern void remove_html_with_class(char *class_name);
extern void display_html_loop(html_t html_formatter, char **elements, int num_elements, bool reverse);
extern void remove_html_children_of_id(char *id);
extern void insert_html_at_id(html_t raw_html, char *id);

/** Global variables
*/
EMSCRIPTEN_WEBSOCKET_T ws = (EMSCRIPTEN_WEBSOCKET_T)NULL;
EmscriptenWebSocketCreateAttributes attr;
char **clobs;
char *curr_clob;
int num_clobs;
bool connected;

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

    emscripten_websocket_init_create_attributes(&attr);

    int isProd = EM_ASM_INT(
        { return window.location.hostname == "127.0.0.1" ? 1 : 0; });

    attr.url = isProd == 1 ? "ws://localhost:3000/" : "wss://clobber-api.azurewebsites.net";

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