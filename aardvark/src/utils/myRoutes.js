export default function buildMenuItem (text, icon, hidden, to, permissions, type, resource, children = null, redirect = null) {
  return {
    text: text,
    icon: icon,
    hidden: hidden,
    to: to,
    permissions: permissions,
    type: type,
    resource: resource,
    children: children,
    redirect: redirect,
  }
}
