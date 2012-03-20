#include <stdio.h>  /* printf */
#include <stdlib.h> /* for calloc, free */
#include <string.h> /* for memcpy */
#include <time.h>   /* for clock */

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

#if defined _MSC_VER
static __inline void swap(uint32 *a, uint32 *b) {
	uint32 t = *a;
	*a = *b;
	*b = t;
}
#else
static inline void swap(uint32 *a, uint32 *b) {
	uint32 t = *a;
	*a = *b;
	*b = t;
}
#endif

void quick_sort(uint32 *values, uint32 begin, uint32 end) {
	if (end > begin + 1) {
		uint32 pivot = values[begin], l = begin + 1, r = end;
		while (l < r) {
			if (values[l] <= pivot) {
				l++;
			} else {
				swap(&values[l], &values[--r]);
			}
		}
		swap(&values[--l], &values[begin]);
		quick_sort(values, begin, l);
		quick_sort(values, r, end);
	}
}

int counting_sort(uint32 *values, uint32 nmemb, uint32 max) {
	uint32 *counts;
	uint32 *tmp;
	uint32 i;
	uint32 *p;

	if (!values) {
		return -1;
	} else if (!nmemb) {
		return -2;
	} else if (!max) {
		return -3;
	} else if (!(counts = calloc(max + 1, sizeof(uint32)))) {
		return -4;
	} else if (!(tmp = calloc(nmemb, sizeof(uint32)))) {
		free(tmp);
		return -5;
	} else {
		/* count up for each values. */
		for (i = 0; i < nmemb; i++) {
			counts[values[i]]++;
		}

		/* count to cumulative frequency */
		for (i = 1; i <= max; i++) {
			counts[i] += counts[i - 1];
		}

		/* arrange */
		p = values + nmemb - 1;
		for (i = 0; i < nmemb; i++, p--) {
			tmp[--counts[*p]] = *p;
		}

		/* copy back */
		memcpy(values, tmp, nmemb * sizeof(uint32));

		free(tmp);
		free(counts);

		return 0;
	}
}

int counting_sort2(uint32 *values, uint32 nmemb, uint32 max) {
	uint32 *counts;
	uint32 *tmp;
	uint32 i;
	uint32 *p;

	if (!values) {
		return -1;
	} else if (!nmemb) {
		return -2;
	} else if (!max) {
		return -3;
	} else if (!(counts = calloc(max + 1, sizeof(uint32)))) {
		return -4;
	} else if (!(tmp = calloc(nmemb, sizeof(uint32)))) {
		free(tmp);
		return -5;
	} else {
		/* count up for each values. */
		for (i = 0; i < nmemb; i++) {
			counts[values[i]]++;
		}

		/* count to cumulative frequency */
		for (i = 1; i <= max; i++) {
			counts[i] += counts[i - 1];
		}

		/* arrange */
		p = values + nmemb - 1;
		for (i = 0; i < nmemb; i++, p--) {
			tmp[--counts[*p]] = *p;
		}

		/* copy back */
		for (i = 0; i < nmemb; i++) {
			values[i] = tmp[i];
		}

		free(tmp);
		free(counts);

		return 0;
	}
}

int counting_sort3(uint32 *values, uint32 nmemb, uint32 max) {
	uint32 *counts;
	uint32 *tmp;
	uint32 i;
	uint32 *p;

	if (!values) {
		return -1;
	} else if (!nmemb) {
		return -2;
	} else if (!max) {
		return -3;
	} else if (!(counts = calloc(max + 1, sizeof(uint32)))) {
		return -4;
	} else if (!(tmp = calloc(nmemb, sizeof(uint32)))) {
		free(tmp);
		return -5;
	} else {
		/* count up for each values. */
		for (i = 0, p = values; i < nmemb; i++, p++) {
			counts[*p]++;
		}

		/* count to cumulative frequency */
		for (i = 1, p = counts; i <= max; i++, p++) {
			*(p + 1) += *p;
		}

		/* arrange */
		p = values + nmemb - 1;
		for (i = 0; i < nmemb; i++, p--) {
			tmp[--counts[*p]] = *p;
		}

		/* copy back */
		memcpy(values, tmp, nmemb * sizeof(uint32));

		free(tmp);
		free(counts);

		return 0;
	}
}

int radix_sort(uint32 *values, uint32 nmemb) {
	uint32 *counts;
	uint32 *tmp;
	uint32 i, j, shift;

	if (!values) {
		return -1;
	} else if (!nmemb) {
		return -2;
	} else if (!(counts = calloc(256, sizeof(uint32)))) {
		return -3;
	} else if (!(tmp = calloc(nmemb, sizeof(uint32)))) {
		free(tmp);
		return -4;
	} else {
		for (j = 0; j < 4; j++) {
			shift = j << 3;

			memset(counts, 0, 256 * sizeof(uint32));

			/* count up for each values. */
			for (i = 0; i < nmemb; i++) {
				counts[(values[i] >> shift) & 0xff]++;
			}

			/* count to cumulative frequency */
			for (i = 1; i < 256; i++) {
				counts[i] += counts[i - 1];
			}

			/* arrange */
			for (i = 0; i < nmemb; i++) {
				tmp[--counts[(values[nmemb - 1 - i] >> shift) & 0xff]] = values[nmemb - 1 - i];
			}

			/* copy back */
			memcpy(values, tmp, nmemb * sizeof(uint32));
		}

		free(tmp);
		free(counts);

		return 0;
	}
}

int radix_sort2(uint32 *values, uint32 nmemb) {
	uint32 *counts;
	uint32 *tmp;
	uint32 i, j, index;
	uint8 *p;
	uint32 *value;

	if (!values) {
		return -1;
	} else if (!nmemb) {
		return -2;
	} else if (!(counts = calloc(256, sizeof(uint32)))) {
		return -3;
	} else if (!(tmp = calloc(nmemb, sizeof(uint32)))) {
		free(tmp);
		return -4;
	} else {
		for (j = 0; j < 4; j++) {
			index = j; /* for little endian */
			/* index = 3 - j; */ /* for little endian */

			memset(counts, 0, 256 * sizeof(uint32));

			/* count up for each values. */
			p = ((uint8 *) values) + index;
			for (i = 0; i < nmemb; i++, p += 4) {
				counts[*p]++;
			}

			/* count to cumulative frequency */
			for (i = 1; i < 256; i++) {
				counts[i] += counts[i - 1];
			}

			/* arrange */
			p = ((uint8 *) (values + nmemb - 1)) + index;
			value = values + nmemb - 1;
			for (i = 0; i < nmemb; i++, p -= 4, value--) {
				tmp[--counts[*p]] = *value;
			}

			/* copy back */
			memcpy(values, tmp, nmemb * sizeof(uint32));
		}

		free(tmp);
		free(counts);

		return 0;
	}
}

int radix_sort3(uint32 *values, uint32 nmemb) {
	uint32 *counts;
	uint32 *tmp;
	uint32 i, j, index;
	uint8 *p;
	uint32 *value;
	uint32 *bufs[2], *src, *dst;

	if (!values) {
		return -1;
	} else if (!nmemb) {
		return -2;
	} else if (!(counts = calloc(256, sizeof(uint32)))) {
		return -3;
	} else if (!(tmp = calloc(nmemb, sizeof(uint32)))) {
		free(tmp);
		return -4;
	} else {
		bufs[0] = values;
		bufs[1] = tmp;
		for (j = 0; j < 4; j++) {
			src = bufs[(j + 0) & 1];
			dst = bufs[(j + 1) & 1];
			index = j; /* for little endian */
			/* index = 3 - j; */ /* for little endian */

			memset(counts, 0, 256 * sizeof(uint32));

			/* count up for each values. */
			p = ((uint8 *) src) + index;
			for (i = 0; i < nmemb; i++, p += 4) {
				counts[*p]++;
			}

			/* count to cumulative frequency */
			for (i = 1; i < 256; i++) {
				counts[i] += counts[i - 1];
			}

			/* arrange */
			p = ((uint8 *) (src + nmemb - 1)) + index;
			value = src + nmemb - 1;
			for (i = 0; i < nmemb; i++, p -= 4, value--) {
				dst[--counts[*p]] = *value;
			}
		}

		free(tmp);
		free(counts);

		return 0;
	}
}

int radix_sort4(uint32 *values, uint32 nmemb) {
	uint32 *counts;
	uint32 *tmp;
	uint32 i, j, index;
	uint16 *p;
	uint32 *value;
	uint32 *bufs[2], *src, *dst;

	if (!values) {
		return -1;
	} else if (!nmemb) {
		return -2;
	} else if (!(counts = calloc(65536, sizeof(uint32)))) {
		return -3;
	} else if (!(tmp = calloc(nmemb, sizeof(uint32)))) {
		free(tmp);
		return -4;
	} else {
		bufs[0] = values;
		bufs[1] = tmp;
		for (j = 0; j < 2; j++) {
			src = bufs[(j + 0) & 1];
			dst = bufs[(j + 1) & 1];
			index = j; /* for little endian */
			/* index = 3 - j; */ /* for little endian */

			memset(counts, 0, 65536 * sizeof(uint32));

			/* count up for each values. */
			p = ((uint16 *) src) + index;
			for (i = 0; i < nmemb; i++, p += 2) {
				counts[*p]++;
			}

			/* count to cumulative frequency */
			for (i = 1; i < 65536; i++) {
				counts[i] += counts[i - 1];
			}

			/* arrange */
			p = ((uint16 *) (src + nmemb - 1)) + index;
			value = src + nmemb - 1;
			for (i = 0; i < nmemb; i++, p -= 2, value--) {
				dst[--counts[*p]] = *value;
			}
		}

		free(tmp);
		free(counts);

		return 0;
	}
}

int cmp_uint(const void *a, const void *b) {
	return *((const uint32 *) a) - *((const uint32 *) b);
}

void sort_qsort(uint32 *values, uint32 nmemb, uint32 max) {
	qsort(values, nmemb, sizeof(uint32), cmp_uint);
}

void sort_quick_sort(uint32 *values, uint32 nmemb, uint32 max) {
	quick_sort(values, 0, nmemb - 1);
}

void sort_counting_sort(uint32 *values, uint32 nmemb, uint32 max) {
	counting_sort(values, nmemb, max);
}

void sort_counting_sort2(uint32 *values, uint32 nmemb, uint32 max) {
	counting_sort2(values, nmemb, max);
}

void sort_counting_sort3(uint32 *values, uint32 nmemb, uint32 max) {
	counting_sort3(values, nmemb, max);
}

void sort_radix_sort(uint32 *values, uint32 nmemb, uint32 max) {
	radix_sort(values, nmemb);
}

void sort_radix_sort2(uint32 *values, uint32 nmemb, uint32 max) {
	radix_sort2(values, nmemb);
}

void sort_radix_sort3(uint32 *values, uint32 nmemb, uint32 max) {
	radix_sort3(values, nmemb);
}

void sort_radix_sort4(uint32 *values, uint32 nmemb, uint32 max) {
	radix_sort4(values, nmemb);
}

int main(int argc, char **argv) {
	struct {
		char *name;
		void (*func)(uint32 *values, uint32 nmemb, uint32 max);
	} benches[] = {
			{ "qsort         ", sort_qsort },
			{ "quick_sort    ", sort_quick_sort },
			{ "counting_sort ", sort_counting_sort },
			{ "counting_sort2", sort_counting_sort2 },
			{ "counting_sort3", sort_counting_sort3 },
			{ "radix_sort    ", sort_radix_sort },
			{ "radix_sort2   ", sort_radix_sort2 },
			{ "radix_sort3   ", sort_radix_sort3 },
			{ "radix_sort4   ", sort_radix_sort4 },
			{ NULL, NULL }
	};
	struct {
		uint32 size;
		uint32 range;
		const char *desc;
		time_t elapsed;
	} test_patters[] = {
		{  320 *  240, 1 << 24, "QVGA         24bpp  320x 240", 0 },
		{  640 *  480, 1 << 24, "VGA          24bpp  640x 480", 0 },
		{ 1024 *  768, 1 << 24, "XGA          24bpp 1024x 768", 0 },
		{  480 *  800, 1 << 24, "Nexus S      24bpp  480x 800", 0 },
		{ 1440 *  900, 1 << 24, "Mac Book Air 24bpp 1440x 900", 0 },
		{ 2560 * 1440, 1 << 24, "iMac         24bpp 2560x1440", 0 },

		{ 0, 0 }
	};

	uint32 i;
	for (i = 0; i < test_patters[i].size; i++) {
		uint32 j;
		time_t t0, t1;
		uint32 *values_src = malloc(test_patters[i].size * sizeof(uint32));
		uint32 *values_dst = malloc(test_patters[i].size * sizeof(uint32));

		printf("%s (size = %u, range = %u)\n",
			test_patters[i].desc, test_patters[i].size, test_patters[i].range);

		for (j = 0; j < test_patters[i].size; j++) {
			values_src[j] = rand();
			values_src[j] %= test_patters[i].range;
		}

		for (j = 0; benches[j].func; j++) {
			memcpy(values_dst, values_src, test_patters[i].size * sizeof(uint32));

			t0 = clock();
			benches[j].func(values_dst, test_patters[i].size, test_patters[i].range);
			t1 = clock();
			printf("%s : %f [sec]\n", benches[j].name,
					((double) t1 - t0) / (double) CLOCKS_PER_SEC);
		}
		printf("\n");

		free(values_dst);
		free(values_src);
	}

	return 0;
}
