/*
 * aos.h - AmigaOS-like macros and definitions
 * Written by Vincent Riviere, 2017.
 * This file is public domain.
 */

#ifndef AOS_H
#define AOS_H

#include "types.h"

struct Node MAY_ALIAS;
struct Node {
	struct Node *ln_Succ;
	struct Node *ln_Pred;
	UBYTE ln_Type;
	BYTE ln_Pri;
	char *ln_Name;
};

struct MinNode MAY_ALIAS;
struct MinNode {
	struct MinNode *mln_Succ;
	struct MinNode *mln_Pred;
};

struct List MAY_ALIAS;
struct List {
	struct Node *lh_Head;
	struct Node *lh_Tail;
	struct Node *lh_TailPred;
	UBYTE lh_Type;
	UBYTE l_pad;
};

struct MinList MAY_ALIAS;
struct MinList {
	struct MinNode *mlh_Head;
	struct MinNode *mlh_Tail;
	struct MinNode *mlh_TailPred;
};

#define NewList(pl) \
do \
{ \
	(pl)->lh_Head = (struct Node *)&(pl)->lh_Tail; \
	(pl)->lh_Tail = NULL; \
	(pl)->lh_TailPred = (struct Node *)(pl); \
} while (0)

#define NewMinList(pml) \
do \
{ \
	(pml)->mlh_Head = (struct MinNode *)&(pml)->mlh_Tail; \
	(pml)->mlh_Tail = NULL; \
	(pml)->mlh_TailPred = (struct MinNode *)(pml); \
} while (0)

#define ForEachNode(pl, pn) \
for \
( \
	(pn) = (pl)->lh_Head; \
	(pn)->ln_Succ; \
	(pn) = (pn)->ln_Succ \
)

#define ForEachMinNode(pml, pmn) \
for \
( \
	(pmn) = (pml)->mlh_Head; \
	(pmn)->mln_Succ; \
	(pmn) = (pmn)->mln_Succ \
)

#endif /* AOS_H */
