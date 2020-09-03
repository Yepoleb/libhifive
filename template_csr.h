#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

{% for en in enums %}
typedef enum {{ en.name }}
{
    {% for enval in en.definitions %}
    {{ enval.name }} = {{ enval.value }},
    {% endfor %}
} {{ en.name }};

{% endfor %}
{% for csr in csrs %}
{% set fullname = "csr_" + csr.name %}

{% if csr.fields|length > 1 %}
typedef struct {{ fullname }}
{
    {% for field in csr.fields %}
    {{ field.type }} {{ field.name }};
    {% endfor %}
} {{ fullname }};

{% if "r" in csr.attr %}
static inline {{ fullname }} {{ fullname }}_get()
{
    {{ fullname }} result;
    uint32_t reg_value;
    __asm__ volatile("csrr %0, {{ csr.name }}" : "=r"(reg_value));
    {% for f in csr.fields if "r" in f.attr %}
    result.{{ f.name }} = ({{ f.type }})((reg_value >> {{ f.start }}) & {{ f.mask }});
    {% endfor %}
    return result;
}

{% endif %}
{% if "w" in csr.attr %}
static inline void {{ fullname }}_set({{ fullname }} values)
{
    uint32_t reg_value = 0;
    {% for f in csr.fields if "w" in f.attr %}
    reg_value |= ((uint32_t){{ f.cast }}(values.{{ f.name }}) & {{ f.mask }}) << {{ f.start }};
    {% endfor %}
    __asm__ volatile("csrw {{ csr.name }}, %0" :: "r"(reg_value));
}

{% endif %}
{% elif csr.fields|length == 1 %}
{% set f = csr.fields[0] %}
{% if "r" in f.attr %}
static inline {{ f.type }} {{ fullname }}_get()
{
    uint32_t reg_value;
    __asm__ volatile("csrr %0, {{ csr.name }}" : "=r"(reg_value));
    return ({{ f.type }})reg_value;
}

{% endif %}
{% if "w" in f.attr %}
static inline void {{ fullname }}_set({{ f.type }} value)
{
    uint32_t reg_value = (uint32_t){{ f.cast }}value;
    __asm__ volatile("csrw {{ csr.name }}, %0" :: "r"(reg_value));
}

{% endif %}
{% endif %}
{% endfor %}
