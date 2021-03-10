static void parseFirstLine(Webs *wp){//parse the HTTP requests
websUrlParse(url, &buf, NULL, &host, &port, &path, &ext, NULL, &query);
wp->path = websValidateUriPath(path);
wp->query = sclone(query);
}
void websSetVar/websSetVarFmt(Webs *wp, char *var, char *fmt, ...){
    v = valueString(sfmtv(fmt, args), 0);
	hashEnter(wp->vars, var, v, 0);//enter the environment variable
}
bool cgiHandler(Webs *wp){
    scopy(cgiPrefix, sizeof(cgiPrefix), wp->path);
   	cgiName = strchr(&cgiPrefix[1], '/');
    dir=cwd;
    cgiPath = sfmt("%s%s/%s", dir, cgiPrefix, cgiName);
    //get cgiPath
    query = sclone(wp->query);
    websDecodeUrl(query, query, strlen(query));
    for (cp = stok(query, " ", &tok); cp != NULL; ) {
        *(argp+n) = cp;   
        n++;
	}//get arguements
    websSetVar(wp, "SCRIPT_FILENAME", cgiPath);
	websSetVar(wp, "PATH_INFO", extraPath);
    websSetVarFmt(wp, "SCRIPT_NAME", "%s/%s", cgiPrefix, cgiName);
    websSetVarFmt(wp, "PATH_TRANSLATED", "%s%s%s", dir, cgiPrefix, extraPath);
    for (n = 0, s = hashFirst(wp->vars); s != NULL; s = hashNext(wp->vars, s))
    {//get value from the table
        envp[n++] = sfmt("%s=%s", s->name.value.string, s->content.value.string);
    }//extract environment variable
    pHandle = launchCgi(cgiPath, argp, envp, stdIn, stdOut);
}
static CgiPid launchCgi(char *cgiPath, char **argp, 
                        char **envp, char *stdIn, char *stdOut){
   	execve(cgiPath, argp, envp);
}

