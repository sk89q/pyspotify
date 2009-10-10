#include <Python.h>
#include <structmember.h>
#include "spotify/api.h"
#include "pyspotify.h"

static PyObject *LinkError;
static PyTypeObject LinkType;

static PyMemberDef Link_members[] = {
    {NULL}
};

static PyObject *Link_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    Link *self;

    self = (Link *)type->tp_alloc(type, 0);
    self->_link = NULL;
    return (PyObject *)self;
}

static PyObject *Link_fromString(Link *self, PyObject *args) {
    fprintf(stderr, "Link_fromString called\n");
    char *s;
    fprintf(stderr, "XXX\n");
    if(!PyArg_ParseTuple(args, "s", &s))
	return NULL;
    fprintf(stderr, "XXX1\n");
    sp_link *link = sp_link_create_from_string(s);
    fprintf(stderr, "XXX2\n");
    if(!link) {
	PyErr_SetString(LinkError, "Failed to get link from a Spotify URI");
	return NULL;
    }
    fprintf(stderr, "XXX3\n");
    Link *plink = (Link *)PyObject_CallObject((PyObject *)&LinkType, NULL);
    fprintf(stderr, "XXX4\n");
    Py_INCREF(plink);
    fprintf(stderr, "XXX5\n");
    plink->_link = link;
    fprintf(stderr, "XXX6\n");
    fprintf(stderr, "Link_fromString completed\n");
    return (PyObject *)plink;
}

static PyObject *Link_fromTrack(Link *self, PyObject *args) {
    PyErr_SetString(PyExc_NotImplementedError, "");
    return NULL;
}

static PyObject *Link_fromAlbum(Link *self, PyObject *args) {
    PyErr_SetString(PyExc_NotImplementedError, "");
    return NULL;
}

static PyObject *Link_fromArtist(Link *self, PyObject *args) {
    PyErr_SetString(PyExc_NotImplementedError, "");
    return NULL;
}

static PyObject *Link_fromSearch(Link *self, PyObject *args) {
    PyErr_SetString(PyExc_NotImplementedError, "");
    return NULL;
}

static PyObject *Link_fromPlaylist(Link *self, PyObject *args) {
    PyErr_SetString(PyExc_NotImplementedError, "");
    return NULL;
}

static PyObject *Link_type(Link *self) {
    PyErr_SetString(PyExc_NotImplementedError, "");
    return NULL;
}

static PyObject *Link_asTrack(Link *self) {
    sp_track *track = sp_link_as_track(self->_link);
    if(!track) {
	PyErr_SetString(LinkError, "Not a track link");
	return NULL;
    }
    Track *ptrack = (Track *)PyObject_CallObject((PyObject *)&TrackType, NULL);
    return (PyObject *)ptrack;
}

static PyObject *Link_asAlbum(Link *self) {
    PyErr_SetString(PyExc_NotImplementedError, "");
    return NULL;
}

static PyObject *Link_asArtist(Link *self) {
    PyErr_SetString(PyExc_NotImplementedError, "");
    return NULL;
}

static PyObject *Link_str(PyObject *oself) {
    Link *self = (Link *)oself;
    fprintf(stderr, "AAA %p\n", self->_link);
    char uri[1024];
    fprintf(stderr, "AAA1\n");
    if(0 > sp_link_as_string(self->_link, uri, sizeof(uri))) {
        fprintf(stderr, "AAA2\n");
	PyErr_SetString(LinkError, "failed to render Spotify URI from link");
	return NULL;
    }
    fprintf(stderr, "AAA3\n");
    return Py_BuildValue("s", uri);
}

static PyMethodDef Link_methods[] = {
    {"fromString",
     (PyCFunction)Link_fromString,
     METH_VARARGS | METH_CLASS,
     "Create a new Link object from a string"},
    {"fromTrack",
     (PyCFunction)Link_fromTrack,
     METH_VARARGS | METH_CLASS,
     "Create a new Link object from a Track object"},
    {"fromAlbum",
     (PyCFunction)Link_fromAlbum,
     METH_VARARGS | METH_CLASS,
     "Create a new Link object from an Album object"},
    {"fromArtist",
     (PyCFunction)Link_fromArtist,
     METH_VARARGS | METH_CLASS,
     "Create a new Link object from an Artist object"},
    {"fromSearch",
     (PyCFunction)Link_fromSearch,
     METH_VARARGS | METH_CLASS,
     "Create a new Link object from a Search object"},
    {"fromPlaylist",
     (PyCFunction)Link_fromPlaylist,
     METH_VARARGS | METH_CLASS,
     "Create a new Link object from a Playlist object"},
    {"type",
     (PyCFunction)Link_type,
     METH_NOARGS,
     "Return the type of the link"},
    {"asTrack",
     (PyCFunction)Link_asTrack,
     METH_NOARGS,
     "Return this link as a Track object"},
    {"asAlbum",
     (PyCFunction)Link_asAlbum,
     METH_NOARGS,
     "Return this link as a Album object"},
    {"asArtist",
     (PyCFunction)Link_asArtist,
     METH_NOARGS,
     "Return this link as an Artist object"},
    {NULL}
};

static PyTypeObject LinkType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "spotify.link.Link",       /*tp_name*/
    sizeof(Link),              /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    0,                         /*tp_dealloc*/  // TODO: IMPLEMENT THIS WITH sp_link_release
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    Link_str,                  /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Link objects",            /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    Link_methods,              /* tp_methods */
    Link_members,              /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,			       /* tp_init */
    0,                         /* tp_alloc */
    Link_new,                  /* tp_new */
};

static PyMethodDef module_methods[] = {
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initlink(void) {
    PyObject *m;

    if(PyType_Ready(&LinkType) < 0)
	return;

    m = Py_InitModule("link", module_methods);
    if(m == NULL)
        return;

    PyEval_InitThreads();
    LinkError = PyErr_NewException("spotify.link.error", NULL, NULL);
    Py_INCREF(LinkError);
    PyModule_AddObject(m, "error", LinkError);
    PyModule_AddObject(m, "Link", (PyObject *)&LinkType);
}
