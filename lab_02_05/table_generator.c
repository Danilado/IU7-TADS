#include "record.h"
#include "record_table.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generate_name(char *dst, size_t length)
{
    for (size_t i = 0; i < length; ++i)
        dst[i] = rand() % ('Z' - 'a') + 'a';
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    int ages[] = {3, 10, 16};

    if (argc < 2)
        return EXIT_FAILURE;

    size_t count = 0;
    int rc;
    char *filename = argv[1];
    FILE *db_file = fopen(filename, "wb");
    if (!db_file)
        return 10;
    fclose(db_file);

    rc = sscanf(argv[2], "%zu", &count);
    if (rc != 1)
        return 20;

    record_table_t table;
    rc = rec_table_init(&table, count);
    if (rc)
        return 30 + rc;

    for (size_t i = 0; i < count; ++i)
    {
        if (i % 10 == 0)
            printf("Generating... %zu/%zu\r", i + 1, count);

        record_t rec;
        record_init(&rec);

        generate_name(rec.play_name, 20);
        generate_name(rec.theatre_name, 20);

        rec.play_type = rand() % (PLAY_TYPE_N - 1) + 1;

        rec.ticket_prices.min_price = rand() % 100000;
        rec.ticket_prices.max_price =
        rec.ticket_prices.min_price + rand() % 30000;

        if (rec.play_type == FAIRYTALE)
            rec.play_attrs.age_restriction = ages[rand() % 3];

        if (rec.play_type == MUSIC)
        {
            rec.play_attrs.musical_attrs.age_restriction = ages[rand() % 3];
            generate_name(rec.play_attrs.musical_attrs.composer, 20);
            generate_name(rec.play_attrs.musical_attrs.country, 20);
            rec.play_attrs.musical_attrs.length = rand() % 600;
            rec.play_attrs.musical_attrs.type =
            rand() % (MUSICAL_TYPE_N - 1) + 1;
        }

        rec_table_push(&table, &rec);
    }

    puts("\nDone!");

    return table_fwrite(&table, filename);
}