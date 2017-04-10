Trace o
Parse Arg 'src=' srcdir 'ext=' html_ext 'ver=' ver_dot 'dest=' dest 'doc=' docdir 'overview=' overview 'comm=' comm 'commset=' commset 'commsos=' commsos 'query=' query 'glossary=' glossary 'appendix1=' appendix1 'appendix2=' appendix2 'appendix3=' appendix3 'appendix4=' appendix4 'appendix7=' appendix7
srcdir = Strip( srcdir )
html_ext = Strip( html_ext )
ver_dot = Strip( ver_dot )
dest = Strip( dest )
docdir = strip( docdir )
overview = Strip( overview )
comm = Strip( comm )
commset = Strip( commset )
commsos = Strip( commsos )
query = Strip( query )
glossary = Strip( glossary )
appendix1 = Strip( appendix1 )
appendix2 = Strip( appendix2 )
appendix3 = Strip( appendix3 )
appendix4 = Strip( appendix4 )
appendix7 = Strip( appendix7 )
'rexx' srcdir'/man2html.rex' HTML_EXT VER_DOT 'TOCSTART >' dest'/index'HTML_EXT
'./manext' overview '>' dest'/overview.man'
'rexx' srcdir'/man2html.rex' HTML_EXT VER_DOT 'OVERVIEW' dest'/overview.man' dest'/index'HTML_EXT '>' dest'/overview'HTML_EXT
'./manext' COMM '>' dest'/comm.man'
'rexx' srcdir'/man2html.rex' HTML_EXT VER_DOT 'COMM' dest'/comm.man' dest'/index'HTML_EXT '>' dest'/comm'HTML_EXT
'./manext' COMMSET '>' dest'/commset.man'
'rexx' srcdir'/man2html.rex' HTML_EXT VER_DOT 'COMMSET' dest'/commset.man' dest'/index'HTML_EXT '>' dest'/commset'HTML_EXT
'./manext' COMMSOS '>' dest'/commsos.man'
'rexx' srcdir'/man2html.rex' HTML_EXT VER_DOT 'COMMSOS' dest'/commsos.man' dest'/index'HTML_EXT '>' dest'/commsos'HTML_EXT
'./manext' QUERY '>' dest'/query.man'
'rexx' srcdir'/man2html.rex' HTML_EXT VER_DOT 'QUERY'    dest'/query.man' dest'/index'HTML_EXT    '>' dest'/query'HTML_EXT
'./manext' GLOSSARY '>' dest'/glossary.man'
'rexx' srcdir'/man2html.rex' HTML_EXT VER_DOT 'GLOSSARY' dest'/glossary.man' dest'/index'HTML_EXT '>' dest'/glossary'HTML_EXT
'./manext' APPENDIX1 '>' dest'/app1.man'
'rexx' srcdir'/man2html.rex' HTML_EXT VER_DOT 'APPENDIX1' dest'/app1.man' dest'/index'HTML_EXT '>' dest'/app1'HTML_EXT
'./manext' APPENDIX2 '>' dest'/app2.man'
'rexx' srcdir'/man2html.rex' HTML_EXT VER_DOT 'APPENDIX2' dest'/app2.man' dest'/index'HTML_EXT '>' dest'/app2'HTML_EXT
'./manext' APPENDIX3 '>' dest'/app3.man'
'rexx' srcdir'/man2html.rex' HTML_EXT VER_DOT 'APPENDIX3' dest'/app3.man' dest'/index'HTML_EXT '>' dest'/app3'HTML_EXT
'./manext' APPENDIX4 '>' dest'/app4.man'
'rexx' srcdir'/man2html.rex' HTML_EXT VER_DOT 'APPENDIX4' dest'/app4.man' dest'/index'HTML_EXT '>' dest'/app4'HTML_EXT
'rexx' srcdir'/man2html.rex' HTML_EXT VER_DOT 'APPENDIX5' docdir'/app5.htm' dest'/index'HTML_EXT '>' dest'/app5'HTML_EXT
'rexx' srcdir'/man2html.rex' HTML_EXT VER_DOT 'APPENDIX6' docdir'/app6.htm' dest'/index'HTML_EXT '>' dest'/app6'HTML_EXT
'./manext' APPENDIX7 '>' dest'/app7.man'
'rexx' srcdir'/man2html.rex' HTML_EXT VER_DOT 'APPENDIX7' dest'/app7.man' dest'/index'HTML_EXT '>' dest'/app7'HTML_EXT
'cp' srcdir'/HISTORY' dest'/history.man'
'rexx' srcdir'/man2html.rex' HTML_EXT VER_DOT 'HISTORY' dest'/history.man' dest'/index'HTML_EXT '>' dest'/history'HTML_EXT
'cp' srcdir'/THE_Help.txt' dest'/quickref.man'
'rexx' srcdir'/man2html.rex' HTML_EXT VER_DOT 'QUICKREF' dest'/quickref.man' dest'/index'HTML_EXT '>' dest'/quickref'HTML_EXT
'rexx' srcdir'/man2html.rex' HTML_EXT VER_DOT 'FDL' docdir'/fdl.htm' dest'/index'HTML_EXT '>' dest'/fdl'HTML_EXT
'rexx' srcdir'/man2html.rex' HTML_EXT VER_DOT 'TOCEND >>' dest'/index'HTML_EXT
Return 0
