//
// LibMPRIS Demo (C) 2007 deadchip, mirsal
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "mpris/list.h"

void
print_list (MPRISList * list)
{
      while (1)
      {
          fprintf (stdout, "String: '%s'\n", (char*)(list->data));
          if (!list->next)
            break;
          list = list->next;
      }

      fprintf (stdout, "\n");
}

int
main (int n_args, char **args)
{
  MPRISList * list = mpris_list_new ();

  fprintf (stdout, "Appending 3 Elements");
  
  mpris_list_append (list, (void*)strdup ("Elmt1"));
  mpris_list_append (list, (void*)strdup ("Elmt2"));
  mpris_list_append (list, (void*)strdup ("Elmt3"));

  fprintf (stdout, "\nSize: %llu\n", (uint64_t)(mpris_list_size (list)));
  print_list (list);

  fprintf (stdout, "Inserting Element after position 1");

  list = mpris_list_head (list);
  mpris_list_insert (list, (void*)strdup ("ElmtInserted"));
  fprintf (stdout, "\nSize: %llu\n", (uint64_t)(mpris_list_size (list)));
  print_list (mpris_list_head (list));

  fprintf (stdout, "Prepending Element");

  list = mpris_list_head (list);
  mpris_list_prepend (list, (void*)strdup ("ElmtPrepended"));
  fprintf (stdout, "\nSize: %llu\n", (uint64_t)(mpris_list_size (list)));
  print_list (mpris_list_head (list));

  fprintf (stdout, "Appending Element");

  list = mpris_list_head (list);
  mpris_list_append (list, (void*)strdup ("ElmtAppended"));
  fprintf (stdout, "\nSize: %llu\n", (uint64_t)(mpris_list_size (list)));
  print_list (mpris_list_head (list));

  mpris_list_free (list, (MPRISListDataDestroy)(free));

  return EXIT_SUCCESS;
}
