String appendIndexIfRoot(String path) {
    if (path.endsWith("/")) path += "index.htm";
    return path;
}

String parseDataType(String path) {
    String dataType = "text/plain";
    if (path.endsWith(".htm"))
        dataType = "text/html; charset=utf-8";
    else if (path.endsWith(".css"))
        dataType = "text/css; charset=utf-8";
    else if (path.endsWith(".js"))
        dataType = "application/javascript";
    else if (path.endsWith(".html"))
        dataType = "image/png";
    else if (path.endsWith(".html"))
        dataType = "image/gif";
    else if (path.endsWith(".html"))
        dataType = "image/jpeg";
    return dataType;
}

String split(String request, int index, const char delim) {
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = request.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (request.charAt(i) == delim || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }

    return found > index ? request.substring(strIndex[0], strIndex[1]) : "";
}

String removeLastChar(String from) {
    return from.substring(0, from.length()-1);
}

int toInt(const char* input) {
    int r = 0;
    for(int i = 0; input[i] != '\0'; ++i) {
        r *= 10;
        r += input[i]-'0';
    }
    return r;
}