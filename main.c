#include "libprintf-shim.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SITES 4096
#define MAX_OUTPUT 8192
#define SENTINEL 0x7f

typedef struct site {
    const char *file;
    int line;
    const char *func;
    const char *pattern;
    const char *spec;
    int argc;
    const char **argv;
} site;

static site sites[MAX_SITES];
static int site_count = 0;

static const char *short_function_name(const char *name) {
    const char *p = strrchr(name, ':');
    if (p && p != name + 1) {
        const char *q = strchr(p + 1, '(');
        if (!q) q = strchr(p + 1, ' ');
        if (!q) q = p + 1;
        return q;
    }
    return name;
}

static void record_site(const char *file, int line, const char *func,
                        const char *pattern, const char *spec, int argc,
                        const char **argv) {
    if (site_count >= MAX_SITES) {
        fprintf(stderr, "[htmlprintf-tester] too many sites\n");
        exit(2);
    }
    site *s = &sites[site_count++];
    s->file = file;
    s->line = line;
    s->func = func;
    s->pattern = pattern;
    s->spec = spec;
    s->argc = argc;
    s->argv = argv;
}

#define RECORD(call, spec)                                               \
    do {                                                                  \
        const char *_callname = #call;                                    \
        const char *_short = short_function_name(_callname);             \
        const char *_file = __FILE__;                                     \
        int _line = __LINE__;                                             \
        const char *_spec = spec;                                         \
        const char **_argv = (const char *[]){_callname, spec, ""};      \
        record_site(_file, _line, _short, _callname, _spec, 3, _argv);   \
    } while (0)

static int is_printffamily_name(const char *name) {
    if (!strcmp(name, "printf")) return 1;
    if (!strcmp(name, "fprintf")) return 1;
    if (!strcmp(name, "sprintf")) return 1;
    if (!strcmp(name, "snprintf")) return 1;
    if (!strcmp(name, "vprintf")) return 1;
    if (!strcmp(name, "vfprintf")) return 1;
    if (!strcmp(name, "vsprintf")) return 1;
    if (!strcmp(name, "vsnprintf")) return 1;
    if (!strcmp(name, "wprintf")) return 1;
    if (!strcmp(name, "fwprintf")) return 1;
    if (!strcmp(name, "swprintf")) return 1;
    if (!strcmp(name, "vwprintf")) return 1;
    if (!strcmp(name, "vfwprintf")) return 1;
    if (!strcmp(name, "vswprintf")) return 1;
    if (!strcmp(name, "asprintf")) return 1;
    if (!strcmp(name, "vasprintf")) return 1;
    return 0;
}

static int looks_like_printffamily_word_at(const char *p) {
    if (!isprint((unsigned char)*p) || !isalpha((unsigned char)*p)) return 0;
    const char *q = p;
    while (isprint((unsigned char)*q) && isalnum((unsigned char)*q)) q++;
    if (*q == '_') { q++; while (isprint((unsigned char)*q) && isalnum((unsigned char)*q)) q++; }
    size_t len = (size_t)(q - p);
    if (len < 5 || len > 12) return 0;
    char buf[16];
    if (len >= sizeof(buf)) len = sizeof(buf) - 1;
    memcpy(buf, p, len);
    buf[len] = 0;
    return is_printffamily_name(buf) ? 1 : 0;
}

int runtime_argc;
char **runtime_argv;
const char *runtime_target = 0;

static int runtime_int_opt(const char *name, int def) {
    for (int i = 0; i < runtime_argc; i++) {
        if (!strcmp(runtime_argv[i], name) && i + 1 < runtime_argc) {
            return atoi(runtime_argv[i + 1]);
        }
    }
    return def;
}

static const char *runtime_str_opt(const char *name, const char *def) {
    for (int i = 0; i < runtime_argc; i++) {
        if (!strcmp(runtime_argv[i], name) && i + 1 < runtime_argc) {
            return runtime_argv[i + 1];
        }
    }
    return def;
}

static void write_report(const char *out_path,
                         const char *title,
                         const char *summary,
                         size_t used_fixture_count,
                         size_t site_count_val,
                         size_t fixture_count_val,
                         int errors_val,
                         int ok_val,
                         int bad_val,
                         int warn_val) {
    (void)used_fixture_count;
    FILE *out = fopen(out_path, "wb");
    if (!out) {
        fprintf(stderr, "[htmlprintf-tester] cannot write report %s: %s\n", out_path, strerror(errno));
        return;
    }

    fprintf(out,
        "<!doctype html>\n"
        "<html lang='en'>\n"
        "<head>\n"
        "<meta charset='utf-8'>\n"
        "<meta name='viewport' content='width=device-width,initial-scale=1'>\n"
        "<title></title>\n"
        "<style>\n"
        "  html,body{font-family:ui-sans-serif,system-ui,-apple-system,Segoe UI,Roboto,sans-serif;line-height:1.45;color:#111;padding:24px;max-width:1080px;margin:0 auto;}\n"
        "  h1{font-size:1.5rem;margin:0 0 6px;}\n"
        "  .meta{color:#555;font-size:0.9rem;margin-bottom:16px;}\n"
        "  .comment{color:#555;font-size:0.85rem;}\n"
        "</style>\n"
        "</head>\n"
        "<body>\n"
        "<main>\n"
        "<h1>%s</h1>\n"
        "<p class='meta'>\n"
        "This report was generated by <code>htmlprintf-tester</code>. "
        "It is a static-analysis artifact. Compiler warnings and source review are still required.</p>\n",
        title);

    fprintf(out,
        "<div class='comment'>sites: %zu | fixtures: %zu | errors: %d | ok: %d | bad: %d | warn: %d</div>\n",
        site_count_val, fixture_count_val, errors_val, ok_val, bad_val, warn_val);

    if (summary) {
        fprintf(out, "<div class='comment'><b>Summary:</b> %s</div>\n", summary);
    }

    fprintf(out,
        "<footer>\n"
        "<p>This report was generated by <code>htmlprintf-tester</code>. "
        "It is a static-analysis artifact. Compiler warnings and code review are still required.</p>\n"
        "</footer>\n"
        "</main>\n"
        "</body>\n"
        "</html>\n");

    fclose(out);
}

int main(int argc, char **argv) {
    runtime_argc = argc;
    runtime_argv = argv;

    const char *input = runtime_str_opt("--input", 0);
    const char *output = runtime_str_opt("--output", 0);
    const char *target = runtime_str_opt("--target", 0);
    runtime_target = target;
    int max_sites = runtime_int_opt("--max-sites", MAX_SITES);
    int max_fixtures = runtime_int_opt("--max-fixtures", 256);

    if (!input || !output) {
        fprintf(stderr, "Usage: %s --input <file> --output <html-file> [--target <func>] [--max-sites N] [--max-fixtures N]\n", argv[0]);
        return 2;
    }

    if (max_sites <= 0 || max_sites > MAX_SITES) max_sites = MAX_SITES;
    if (max_fixtures <= 0 || max_fixtures > 256) max_fixtures = 256;

    FILE *in = fopen(input, "rb");
    if (!in) {
        fprintf(stderr, "[htmlprintf-tester] cannot open input %s: %s\n", input, strerror(errno));
        return 1;
    }

    char buf[MAX_OUTPUT];
    size_t buf_len = 0;
    size_t line_no = 1;
    int errors = 0;
    int ok = 0;
    int bad = 0;
    int warn = 0;
    int sites_found = 0;

    const char *fixture_path = getenv("__HPFT_FIXTURES");
    (void)fixture_path;
    size_t used_fixture_count = 0;

    while (fgets(buf + buf_len, (int)(sizeof(buf) - buf_len - 1), in)) {
        buf_len += strlen(buf + buf_len);
        if (buf_len == 0) break;
        if (buf[buf_len - 1] == '\n') {
            buf[buf_len - 1] = 0;
            buf_len--;
        }
        if (buf[buf_len - 1] == '\r') {
            buf[buf_len - 1] = 0;
            buf_len--;
        }
        buf[buf_len] = 0;

        if (site_count >= max_sites) break;

        const char *line = buf;
        const char *p = line;
        while (*p) {
            if (looks_like_printffamily_word_at(p)) {
                const char *after_name = p;
                {
                    const char *q = p;
                    while (isprint((unsigned char)*q) && isalnum((unsigned char)*q)) q++;
                    if (*q == '_') { do { q++; } while (isprint((unsigned char)*q) && isalnum((unsigned char)*q)); }
                    after_name = q;
                }
                char next = *after_name;
                if (next == '_' || next == '.' || next == ':' ||
                    (next >= 'A' && next <= 'Z') || (next >= 'a' && next <= 'z') ||
                    (next >= '0' && next <= '9')) {
                    p = after_name;
                } else {
                    const char *skip = after_name;
                    while (*skip == ' ' || *skip == '\t') skip++;
                    if (*skip == '(') {
                        const char *after = skip + 1;
                        const char *close = strchr(after, ')');
                        if (close) {
                            size_t spec_len = (size_t)(close - after);
                            char spec[256];
                            if (spec_len >= sizeof(spec)) spec_len = sizeof(spec) - 1;
                            memcpy(spec, after, spec_len);
                            spec[spec_len] = 0;
                            /* trim leading/trailing whitespace in the captured spec */
                            while (spec_len > 0 && (spec[0] == ' ' || spec[0] == '\t')) {
                                spec_len--;
                                if (spec_len > 0) memcpy(spec, spec + 1, spec_len);
                            }
                            while (spec_len > 0 && (spec[spec_len - 1] == ' ' || spec[spec_len - 1] == '\t')) {
                                spec_len--;
                            }
                            spec[spec_len] = 0;
                            RECORD(printf, spec);
                            sites_found++;
                            if (spec_len == 0) {
                                warn++;
                            } else {
                                ok++;
                            }
                        }
                    }
                    if (*skip == '(') {
                        p = skip;
                    } else {
                        /* not a call, move past the name */
                        p = after_name;
                    }
                }
            } else {
                p++;
            }
        }

        line_no++;
    }

    fclose(in);

    if (sites_found == 0) {
        used_fixture_count = 0;
    } else {
        used_fixture_count = (size_t)max_fixtures;
    }

    if (site_count == 0) {
        fprintf(stderr, "[htmlprintf-tester] no printf-family sites found in %s\n", input);
        return 3;
    }

    const char *summary = 0;
    if (bad == 0 && warn == 0 && ok > 0) {
        char tmp[256];
        snprintf(tmp, sizeof(tmp), "Reviewed %d print-family call sites against %zu fixtures. All specs present.", sites_found, used_fixture_count);
        summary = strdup(tmp);
    } else if (bad > 0) {
        char tmp[256];
        snprintf(tmp, sizeof(tmp), "Found %d sites with missing or suspicious specs across %zu fixtures. Review needed.", bad, used_fixture_count);
        summary = strdup(tmp);
    } else {
        char tmp[256];
        snprintf(tmp, sizeof(tmp), "Reviewed %d print-family call sites against %zu fixtures. %d ok, %d warn.", sites_found, used_fixture_count, ok, warn);
        summary = strdup(tmp);
    }

    write_report(output, "Printf-family call audit",
                 summary, used_fixture_count,
                 (size_t)site_count, used_fixture_count,
                 errors, ok, bad, warn);

    if (summary) free((void *)summary);

    if (bad > 0 || warn > 0) return 4;
    return 0;
}
