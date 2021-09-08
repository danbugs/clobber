EM_BOOL wss_on_open(int eventType, const EmscriptenWebSocketOpenEvent *e, void *userData)
{
    printf("opened the connection!\n");
    connected = true;
    return 0;
}

EM_BOOL wss_on_close(int eventType, const EmscriptenWebSocketCloseEvent *e, void *userData)
{
    printf("closed the connection!\n");
    connected = false;
    return 0;
}

EM_BOOL wss_on_error(int eventType, const EmscriptenWebSocketErrorEvent *e, void *userData)
{
    printf("something went wrong!\n");
    return 0;
}

EM_BOOL wss_on_message(int eventType, const EmscriptenWebSocketMessageEvent *e, void *userData)
{
    clobs = split((char *)e->data, "\n", &num_clobs);
    get_all_clobs();
    return 0;
}