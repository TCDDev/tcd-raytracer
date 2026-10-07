#!/usr/bin/env bash

set -euo pipefail

name="$1"
file="include/raytracer/${name}.hpp"

cat > "$file" <<EOF
#pragma once

namespace raytracer {


}
EOF

echo "Created $file"