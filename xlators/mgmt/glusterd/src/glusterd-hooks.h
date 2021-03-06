/*
  Copyright (c) 2006-2012 Gluster, Inc. <http://www.gluster.com>
  This file is part of GlusterFS.

  GlusterFS is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published
  by the Free Software Foundation; either version 3 of the License,
  or (at your option) any later version.

  GlusterFS is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <http://www.gnu.org/licenses/>.
*/

#ifndef _GLUSTERD_HOOKS_H_
#define _GLUSTERD_HOOKS_H_

#ifndef _CONFIG_H
#define _CONFIG_H
#include "config.h"
#endif

#define GLUSTERD_GET_HOOKS_DIR(path, version, priv) \
        snprintf (path, PATH_MAX, "%s/hooks/%d", priv->workdir,\
                  version);

#define GLUSTERD_HOOK_VER       1

typedef enum glusterd_commit_hook_type {
        GD_COMMIT_HOOK_NONE = 0,
        GD_COMMIT_HOOK_PRE,
        GD_COMMIT_HOOK_POST,
        GD_COMMIT_HOOK_MAX
} glusterd_commit_hook_type_t;

typedef struct hooks_private {
        struct list_head        list;
        int                     waitcount; //debug purposes
        pthread_mutex_t         mutex;
        pthread_cond_t          cond;
        pthread_t               worker;
} glusterd_hooks_private_t;

typedef struct hooks_stub {
        struct list_head        all_hooks;
        char                    *scriptdir;
        glusterd_op_t           op;
        dict_t                  *op_ctx;

} glusterd_hooks_stub_t;


int
glusterd_hooks_create_hooks_directory (char *basedir);

char *
glusterd_hooks_get_hooks_cmd_subdir (glusterd_op_t op);

int
glusterd_hooks_run_hooks (char *hooks_path, glusterd_op_t op, dict_t *op_ctx,
                          glusterd_commit_hook_type_t type);
int
glusterd_hooks_spawn_worker (xlator_t *this);

int
glusterd_hooks_stub_init (glusterd_hooks_stub_t **stub, char *scriptdir,
                          glusterd_op_t op, dict_t *op_ctx);
void
glusterd_hooks_stub_cleanup (glusterd_hooks_stub_t *stub);

int
glusterd_hooks_post_stub_enqueue (char *scriptdir, glusterd_op_t op,
                                      dict_t *op_ctx);
int
glusterd_hooks_priv_init (glusterd_hooks_private_t **new);
#endif
