#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {{ devicename }}
{
    volatile uint32_t* const p;
} {{ devicename }};

{% for inst in instances %}
static const {{ devicename }} {{ inst.name }} = {(uint32_t*)({{ inst.address }}ul)};
{% endfor %}
{% if instances|length > 1 %}
{% set instancearg1 %}{{ devicename }} dev{% endset %}
{% set instancearg2 %}{{ devicename }} dev, {% endset %}
{% set instancename %}dev{% endset %}
{% else %}
{% set instancearg1 %}void{% endset %}
{% set instancearg2 %}{% endset %}
{% set instancename %}{{ instances[0].name }}{% endset %}
{% endif %}

{% for en in enums %}
typedef enum {{ en.name }}
{
    {% for enval in en.definitions %}
    {{ enval.name }} = {{ enval.value }},
    {% endfor %}
} {{ en.name }};
{% endfor %}

{% for reg in registers %}
{% set fullname = devicename + "_" + reg.name %}
/*
 * Register {{ fullname }} at byte offset {{ reg.offset_attr }}
 */

#define {{ fullname.upper() }}_OFFSET {{ reg.offset }}

{% if reg.fields|length > 1 %}
typedef struct {{ fullname }}
{
    {% for field in reg.fields %}
    {{ field.type }} {{ field.name }};
    {% endfor %}
} {{ fullname }};

{% if "r" in reg.attr %}
static inline {{ fullname }} {{ fullname }}_get({{ instancearg1 }})
{
    {{ fullname }} result;
    uint32_t reg_value = {{ instancename }}.p[{{ reg.offset }}];
    {% for f in reg.fields if "r" in f.attr %}
    result.{{ f.name }} = ({{ f.type }})((reg_value >> {{ f.start }}) & {{ f.mask }});
    {% endfor %}
    return result;
}

{% endif %}
{% if "w" in reg.attr %}
static inline void {{ fullname }}_set({{ instancearg2 }}{{ fullname }} values)
{
    uint32_t reg_value = 0;
    {% for f in reg.fields if "w" in f.attr %}
    reg_value |= ((uint32_t){{ f.cast }}(values.{{ f.name }}) & {{ f.mask }}) << {{ f.start }};
    {% endfor %}
    {{ instancename }}.p[{{ reg.offset }}] = reg_value;
}

{% endif %}
{% elif reg.fields|length == 1 %}
{% set f = reg.fields[0] %}
{% if "r" in f.attr %}
static inline {{ f.type }} {{ fullname }}_get({{ instancearg1 }})
{
    return ({{ f.type }}){{ instancename }}.p[{{ reg.offset }}];
}

{% endif %}
{% if "w" in f.attr %}
static inline void {{ fullname }}_set({{ instancearg2 }}{{ f.type }} value)
{
    {{ instancename }}.p[{{ reg.offset }}] = (uint32_t){{ f.cast }}value;
}

{% endif %}
{% endif %}
{% endfor %}
