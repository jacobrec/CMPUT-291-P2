/* node.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_DTD_H
#define __LIBXMLPP_DTD_H

#include <libxml++/attribute.h>
#include <list>
#include <map>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern "C" {
  struct _xmlDtd;
}
#endif //DOXYGEN_SHOULD_SKIP_THIS4

namespace xmlpp
{

/** Represents XML DTDs.
 *
 */
class Dtd //TODO: Derive from Node?
{
public:
  Dtd(_xmlDtd* dtd);
  ~Dtd();

  Glib::ustring get_name() const;
  Glib::ustring get_external_id() const;
  Glib::ustring get_system_id() const;
  
private:
  _xmlDtd* impl_;
};

} // namespace xmlpp

#endif //__LIBXMLPP_DTD_H



