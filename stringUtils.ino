String appendIndexIfRoot(String path) {
    if (path.endsWith("/")) path += "index.htm";
    return path;
}

String parseDataType(String filename) {
    if (filename.endsWith(".htm")) {
        return "text/html; charset=utf-8";
    } else if (filename.endsWith(".css")) {
        return "text/css; charset=utf-8";
    } else if (filename.endsWith(".js")) {
        return "application/javascript; charset=utf-8";
    } else if (filename.endsWith(".png")) {
        return "image/png";
    } else if (filename.endsWith(".gif")) {
        return "image/gif";
    } else if (filename.endsWith(".jpg")) {
        return "image/jpeg";
    } else if (filename.endsWith(".ico")) {
        return "image/x-icon";
    } else if (filename.endsWith(".xml")) {
        return "text/xml";
    } else if (filename.endsWith(".pdf")) {
        return "application/x-pdf";
    } else if (filename.endsWith(".zip")) {
        return "application/x-zip";
    } else if (filename.endsWith(".gz")) {
        return "application/x-gzip";
    }
    return "text/plain; charset=utf-8";
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
    return from.substring(0, from.length() - 1);
}

int toInt(const char* input) {
    int r = 0;
    for (int i = 0; input[i] != '\0'; ++i) {
        r *= 10;
        r += input[i] - '0';
    }
    return r;
}