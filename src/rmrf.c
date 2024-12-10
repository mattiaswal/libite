/* Recursive removal of files/directories
 *
 * Copyright (c) 2024  Mattias Walström <lazzer@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * @file rmrf.c
 * @author Mattias Walström
 * @date 2024
 * @copyright ISC License
 */

#include <ftw.h>

#include "lite.h"

static int do_delete(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftw)
{
	if (ftw->level == 0)
		return 1;

	if (remove(fpath) && errno != EBUSY)
		warn("Failed removing %s", fpath);

	return 0;
}

/**
 * Recursive removal of files/directories
 * @param path  File or directory to be removed
 *
 * @returns POSIX OK(0)
 */
int rmrf(const char *path)
{
	if (!fisdir(path))
		return 0;

	nftw(path, do_delete, 64, FTW_DEPTH | FTW_PHYS);
	if (remove(path) && errno != ENOENT)
		warn("Failed removing path %s", path);

	return 0;
}