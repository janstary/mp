#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <err.h>

struct word {
	char*		word;
	struct word*	next;
};

struct list {
	struct word*	head;
	struct word*	tail;
};

void
add(char* this, struct list* list)
{
	struct word *word;
	if ((word = calloc(1, sizeof(struct word))) == NULL)
		err(1, NULL);
	word->word = strdup(this);
	if (list->tail) {
		list->tail->next = word;
		list->tail = word;
	} else {
		list->head = word;
		list->tail = word;
	}
}

int
ismp(char *a, char *b)
{
	int r = 0;
	int la = 0;
	int lb = 0;
	wchar_t wa;
	wchar_t wb;
	char *p,*q;
	for (p = a, q = b; *p && *q; ) {
		if ((la = mbtowc(&wa, p, 4)) == -1
		||  (lb = mbtowc(&wb, q, 4)) == -1)
			return 0;
		if (wmemcmp(&wa, &wb, 1)) {
			if (r)
				return 0;
			r = 1;
		}
		p += la;
		q += lb;
	}
	return (!*p && !*q) ? r : 0;
}

int
main(int argc, char** argv)
{

	char *n;
	FILE * in;
	ssize_t len;
	size_t size = 0;
	char *this = NULL;
	struct word *word;
	struct list *list;

	switch (argc) {
		case 1:
			in = stdin;
			break;
		case 2:
			if ((in = fopen(argv[1], "r")) == NULL)
				err(1, "%s", argv[1]);
			break;
		default:
			fprintf(stderr, "usage: mp [file]\n");
			return 1;
	}
	if (setlocale(LC_CTYPE, "en_US.UTF-8") == NULL)
		err(1, NULL);
	if ((list = calloc(1, sizeof(struct list))) == NULL)
		err(1, NULL);
	while ((len = getline(&this, &size, in)) != -1) {
		if ((n = strchr(this, '\n')))
			*n = '\0';
		for (word = list->head; word; word = word->next) {
			if (ismp(word->word, this))
				printf("%s %s\n", word->word, this);
		}
		add(this, list);
	}
	free(this);
	return 0;
}
