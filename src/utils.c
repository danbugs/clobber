char **split(char *string, char *separator, int *num_elements)
{
    int counter = 1;
    char *tmp = string;

    while (*tmp)
    {
        if (*tmp == *separator)
        {
            counter++;
        }
        tmp++;
    }

    char **result = malloc(sizeof(char *) * counter);
    char *token = strtok(string, separator);

    int index = 0;
    while (token)
    {
        *(result + index++) = strdup(token);
        token = strtok(0, separator);
    }
    *(result + index) = 0;

    *num_elements = counter;
    return result;
}