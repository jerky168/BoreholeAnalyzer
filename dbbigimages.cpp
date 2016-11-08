#include "precompiled.h"
#include "dbbigimages.h"
#include <QxMemLeak.h>

QX_REGISTER_CPP(DbBigImages)

namespace qx {
template <> void register_class(QxClass<DbBigImages> & t)
{
  t.setName("bigImages");

  t.id(& DbBigImages::height, "id");
  t.data(& DbBigImages::imgData, "data");
}
}
