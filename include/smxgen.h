/**
 * Generates the Streamix C code
 *
 * @file    smxgen.h
 * @author  Simon Maurer
 */

#ifndef SMXGEN_H
#define SMXGEN_H

#include <igraph.h>
#include <stdbool.h>
#include "path_defines.h"

#ifndef SMXRTSP_VERSION_LIB
#define SMXRTSP_VERSION_LIB "unknown"
#endif

#define AUTHOR_PATTERN        "<author>"
#define APP_INC_PATTERN       "<app_includes>"
#define APP_NW_PATTERN        "<smx_network>"
#define BIN_NAME_PATTERN      "<bin_name>"
#define APP_DEP_PATTERN       "<app_dependencies>"
#define BOX_REL_PATTERN       "<box_relations>"
#define BOX_LIB_PATTERN       "<lib_name>"
#define BOX_MSG_PATTERN       "<box_msg_types>"
#define SMX_APP_NAME_PATTERN  "<app_name>"
#define SMXC_DEP_PATTERN      "<smxc_dependency>"
#define SMXRTSP_DEP_PATTERN   "<smxrtsp_dependency>"

#define TPL_PATH_APP        TPL_PATH "/smxappgen-" SMXRTSP_VERSION_LIB
#define TPL_PATH_APP_DPKG   TPL_PATH_APP "/debian"
#define TPL_APP_GITIGNORE   TPL_PATH_APP "/_gitignore"
#define TPL_APP_README      TPL_PATH_APP "/README_md"
#define TPL_APP_MK          TPL_PATH_APP "/app_mk"
#define TPL_APP_CONF_MK     TPL_PATH_APP "/config_mk"
#define TPL_APP_LOG         TPL_PATH_APP "/app_zlog"
#define TPL_APP_MAIN        TPL_PATH_APP "/main_c"

/**
 * Read an app template file, replace the generic patterns and insert it to the
 * target file.
 *
 * @param g         pointer to the dependency graph
 * @param tpl_path  path to the template file
 * @param tgt_path  path to the target file
 */
void smxgen_app_file( igraph_t* g, const char* tpl_path, const char* tgt_path );

/**
 * Traverse a directory tree and apply smxgen_app_file() on each file.
 *
 * @param g
 *  a pointer to the dependency graph
 * @param src_path
 *  the path to the source directory
 * @param tgt_path
 *  the path to the target directory
 * @param cp_path
 *  an optional path to copy the generated files to
 * @return
 *  0 on success, -1 on failure.
 */
int smxgen_app_file_tree( igraph_t* g, char* src_path, const char* tgt_path,
        const char* cp_path );

/**
 * Generate the code to conncet an edge to a vertex given a mode.
 *
 * @param g         pointer to the dependency graph
 * @param ident     identation string
 * @param eid       id of the graph edge
 * @param vid       id of the vertex
 * @param mode      the mode of the port
 * @param is_dyn    flag to indicate wether to conect a dynamic (true) or a
 *                  static (false) port.
 */
void smxgen_connect( igraph_t* g, int ident, int eid, int vid, int mode,
        bool is_dyn );

/**
 * Extract box degrees from dependency graph.
 *
 * @param g
 *  A pointer to the dependecy graph.
 * @param id
 *  The vertex ID.
 * @param indeg
 *  An output parameter to store the indegree.
 * @param outdeg
 *  An output parameter to store the outdegree.
 */
void smxgen_get_box_degrees( igraph_t* g, int id, int* indeg, int* outdeg );

/**
 *
 */
void smxgen_get_box_deps( igraph_t* g, char* deps, char* rels );

/**
 * Get the name of a port given the edge id and the mode of the port.
 *
 * @param g     pointer to the dependency graph
 * @param eid   id of the graph edge
 * @param mode  the mode of the port
 * @return      the name of the port or NULL
 */
const char* smxgen_get_port_name( igraph_t* g, int eid, int mode );

/**
 * Insert port templates to the target file.
 *
 */
int smxgen_insert_ports( igraph_t* g, int id, igraph_neimode_t mode,
    const char* box_name, const char* box_path );

/**
 * Generate the smx network of the application.
 *
 * @param g         pointer to the dependency graph
 * @param ident     identation string
 * @param tt_vcnt   pointer to the tt vertice counter
 * @param tt_ecnt   pointer to the tt edge counter
 */
void smxgen_network_create( igraph_t* g, int ident, int* tt_vcnt, int* tt_ecnt );

/**
 * Generate the temporal firewalls
 *
 * @param g         pointer to the dependency graph
 * @param ident     identation string
 * @param eid       the id of the edge with timing annotation
 * @param edge_cnt  the number of edges connected
 * @param ch_len    the length of the annotated edge
 * @param tt        pointer to an array of all timer structures
 * @param tt_vcnt   pointer to the tt vertice counter
 * @param tt_ecnt   pointer to the tt edge counter
 */
void smxgen_network_create_timer( igraph_t* g, int ident, igraph_integer_t eid,
        int edge_cnt, int ch_len, struct timespec* tt, int* tt_vcnt,
        int* tt_ecnt );

/**
 * Generate thi instructions to destroy the complete smx network
 *
 * @param g         pointer to the dependency graph
 * @param ident     identation string
 * @param tt_vcnt   the tt vertice counter
 * @param tt_ecnt   the tt edge counter
 */
void smxgen_network_destroy( igraph_t* g, int ident, int tt_vcnt, int tt_ecnt );

/**
 * Generate the instructions to run the network
 *
 * @param g         pointer to the dependency graph
 * @param ident     identation string
 * @param tt_vcnt   the tt vertice counter
 */
void smxgen_network_run( igraph_t* g, int ident, int tt_vcnt );

/**
 * Get the degree of dynamic ports of a vertice.
 *
 * @param g
 *  A pointer to the dependency graph
 * @param vid
 *  The id of the net
 * @param mode
 *  Constant giving the type of the incident edges to select. Possible values:
 *  IGRAPH_OUT, outgoing edges; IGRAPH_IN, incoming edges; IGRAPH_ALL, all
 *  edges.
 */
int smxgen_newtork_get_dyn_degree( igraph_t* g, int vid,
        igraph_neimode_t mode );

/**
 * Generate the instructions to sync the network on termination
 *
 * @param g         pointer to the dependency graph
 * @param ident     identation string
 * @param tt_vcnt   the tt vertice counter
 */
void smxgen_network_wait_end( igraph_t* g, int ident, int tt_vcnt );

/**
 * Generates the include in the main file.
 *
 * @param g     pointer to the dependency graph
 */
void smxgen_main_includes( igraph_t* g );

/**
 * Checks wether a timer with the same settings already exists
 *
 * @param tt_elem   the timer structure to check
 * @param tt        pointer to an array of all timer structures
 * @param len       the number of existing tt elements
 * @return          1 if a timer already exists, 0 otherwise
 */
int smxgen_timer_is_duplicate( struct timespec tt_elem, struct timespec* tt,
        int len );

/**
 * Copies the box template files to the output and replaces the generic patterns
 * with concrete content.
 *
 * @param g          pointer to the dependency graph
 * @param box_path   path to the folder holding the box implementations
 * @param build_path path to the build folder
 */
void smxgen_tpl_box( igraph_t* g, char* box_path, char* build_path );

/**
 * Copies the main c file to the output and replaces the generic patterns.
 *
 * @param g
 *  A pointer to the dependency graph
 * @param build_path
 *  The path to the build folder
 * @param version
 *  The Streamix app version
 * @param dash_version
 *  The version of the dash frontend.
 * @param schema_defs
 *  A string holding additional app schema definitions. The string is of the
 *  form "<def_1>,...,<def_n>" wehere each definition string is of the form
 *  "<def_name>:<def_pkg_name>:<def_version>".
 */
void smxgen_tpl_main( igraph_t* g, char* build_path, const char* version,
        const char* dash_version, const char* schema_defs );

#endif /* SMXGEN_H */
