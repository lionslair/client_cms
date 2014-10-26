<?php

/**
 * Build the administration menu as renderable menu links.
 *
 * @param $tree
 *   A data structure representing the administration menu tree as returned from
 *   menu_tree_all_data().
 *
 * @return
 *   The complete Cleint CMS menu, suitable for theme_admin_menu_links().
 *
 */
function _build_client_cms_links($tree) {
  $links = array();
  foreach ($tree as $data) {

    if ($data['link']['href'] == '<nolink>') {
      $menu_key = $data['link']['href'] . '-' . $data['link']['weight'];
    }
    else {
      $menu_key = $data['link']['href'];
    }

    // Skip items that are inaccessible, invisible, or link to their parent.
    // (MENU_DEFAULT_LOCAL_TASK), and MENU_CALLBACK-alike items that should only
    // appear in the breadcrumb.
    if (!$data['link']['access'] || $data['link']['type'] & MENU_LINKS_TO_PARENT || $data['link']['type'] == MENU_VISIBLE_IN_BREADCRUMB || $data['link']['hidden'] == 1) {
      continue;
    }

    // Omit alias lookups.
    $data['link']['localized_options']['alias'] = TRUE;
    // Remove description to prevent mouseover tooltip clashes.
    unset($data['link']['localized_options']['attributes']['title']);

    // Make action links (typically "Add ...") appear first in dropdowns.
    // They might appear first already, but only as long as there is no link
    // that comes alphabetically first (e.g., a node type with label "Ad").
    if ($data['link']['type'] & MENU_IS_LOCAL_ACTION) {
      $data['link']['weight'] -= 1000;
    }

    $links[$menu_key] = array(
      '#title' => $data['link']['title'],
      '#href' => $data['link']['href'],
      '#options' => $data['link']['localized_options'],
      '#weight' => $data['link']['weight'],
    );

    // Recurse to add any child links.
    $children = array();
    if ($data['below']) {
      $children = _build_client_cms_links($data['below']);
      $links[$menu_key] += $children;
    }

    // Handle links pointing to category/overview pages.
    if ($data['link']['page_callback'] == 'system_admin_menu_block_page' || $data['link']['page_callback'] == 'system_admin_config_page') {
      // Apply a marker for others to consume.
      $links[$data['link']['href']]['#is_category'] = TRUE;
      // Automatically hide empty categories.
      // Check for empty children first for performance. Only when non-empty
      // (typically 'admin/config'), check whether children are accessible.
      if (empty($children) || !element_get_visible_children($children)) {
        $links[$data['link']['href']]['#access'] = FALSE;
      }
    }
  }
  return $links;
}

function _client_cms_get_company_email() {
  $mail = variable_get('client_cms_webshop_email', variable_get('site_mail', 'contact@domain.com'));
  $company = variable_get('client_cms_company_name', variable_get('site_name', 'Site Name'));
  return "$company  <$mail>";
}

/**
 * Create HTML content for tree menu, which can be handled by
 * smoothmenu
 *
 * @param $items
 *  Filled out array of menu items, default view in _springify_get_tree_items()
 * @param $parent_link
 *  If menu has sub-menus this flag can disable parent menu link
 */
function theme_client_cms_main_menu() {
  $output = '<div id="navigation" class="ddsmoothmenu">';
  $rendered = menu_tree('main-menu');
  $output .= render($rendered);
  $output .= '</div>';
  return $output;
}
