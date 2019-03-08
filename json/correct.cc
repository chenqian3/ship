#include <cstdio>
#include <cstring>

using namespace std;

bool check_H_line_remove_coma(char *ln)
{
  if (ln == nullptr) return false;
  int n = strlen(ln);
  int i0 = 0;
  for (int i = 0; i < n; ++i) {
    int c = ln[i];
    if (c != '\n' && c != ' ' && c != '\t' && c != '\r') {
      i0 = i;
      break;
    }
  }
  if (strncmp(ln + i0, "\"H\"", 3) == 0) {
    for (int i = n - 1; i >= 0; --i) {
      int c = ln[i];
      if (c != '\n' && c != ' ' && c != '\t' && c != '\r') {
	if (c == ',') ln[i] = ' ';
	return true;
	break;
      }
    }
  }
  return false;
}

bool only_contain_c(char const *ln, int c)
{
  if (ln == nullptr) return false;
  int n = strlen(ln);
  int nc = 0;
  int c0 = 0;
  for (int i = 0; i < n; ++i) {
    int tc = ln[i];
    if (tc != '\n' && tc != ' ' && tc != '\t' && tc != '\r') {
      c0 = tc;
      ++nc;
    }
  }
  if (nc == 1 && c0 == c) return true; 
  return false;
}

int main()
{
  char buf[1024];
  while (fgets(buf, 1024 - 1, stdin) != nullptr) {
    bool is_H_line = check_H_line_remove_coma(buf);
    fputs(buf, stdout);
    if (is_H_line) {
      if (fgets(buf, 1024 - 1, stdin) != nullptr) {
	if (only_contain_c(buf, '}')) {}
	else fputs(buf, stdout);
      }
    }
  }
  return 0;
}
      
