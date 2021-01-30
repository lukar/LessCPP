#!/bin/sh


echo "Insert the context name: "

read context_name

alluppercase=${context_name^^}
definename=${alluppercase// /_}_CONTEXT_LESSCPP

tmp=( $context_name )
fst_capitalized=${tmp[@]^}
classname=${fst_capitalized// /}Context

alllowercase=${context_name,,}
filename=${alllowercase// /_}

cat >${filename}.h << EOF
#ifndef ${definename}
#define ${definename}

#include "context.h"

class ${classname} : public Context {
public:
    ${classname}();

    void update(const float dt, const sf::Vector2f & mousepos) override { m_dt = dt; m_mousepos = mousepos; };
    Context* processEvent(const sf::Event &) override;
    Context* processBackgroundTask() override;
    sf::Texture render() override;
};


#endif // ${definename}
EOF

cat >${filename}.cc << EOF
#include "${filename}.h"

${classname}::${classname}() {}

Context* ${classname}::processBackgroundTask() { return nullptr; }

Context* ${classname}::processEvent(const sf::Event & event)
{
    return nullptr;
}

sf::Texture ${classname}::render()
{
    rentex.clear();

    rentex.display();

    return rentex.getTexture();
}
EOF
