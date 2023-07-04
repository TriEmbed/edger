export default function buildMenuItem (text, icon, hidden, to, permissions, type, resource, children = null, redirect = null, temporary = false) {
  return {
    text: text,
    icon: icon,
    hidden: hidden,
    to: to,
    temporary: temporary,
    permissions: permissions,
    type: type,
    resource: resource,
    children: children,
    redirect: redirect,
  }
}
