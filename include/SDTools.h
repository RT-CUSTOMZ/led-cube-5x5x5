/*
 * SDTools.h
 *
 *  Created on: 11.08.2013
 *      Author: cutter
 */

#ifndef SDTOOLS_H_
#define SDTOOLS_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t SDParseAndSortDirectory(char directoryEntrys[][13], ssize_t count,
		const char *directory, uint_fast8_t attribute_filter,
		const char *name_filter);

typedef void (*SDIteratorFunction)(char *filename, void *user);
uint32_t SDIterateDirectory(const char *directory, uint_fast8_t attribute_filter,
		const char *name_filter, SDIteratorFunction iterator, void *user);

bool SDReadUIntConfigFile(const char* filename, uint32_t *value);

#endif /* SDTOOLS_H_ */
