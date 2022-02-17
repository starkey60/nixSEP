#pragma once

#include <memory>

#include "types.hh"

namespace nix {

struct Regex;

struct DrvName
{
    string fullName;
    string name;
    string version;
    unsigned int hits;

    DrvName();
    DrvName(std::string_view s);
    ~DrvName();

    bool matches(const DrvName & n);

private:
    std::unique_ptr<Regex> regex;
};

typedef list<DrvName> DrvNames;

std::string_view nextComponent(std::string_view::const_iterator & p,
    const std::string_view::const_iterator end);
int compareVersions(const std::string_view v1, const std::string_view v2);
DrvNames drvNamesFromArgs(const Strings & opArgs);

}
