<?php


/**
 * Generates the link for the admin menu into 3 levels.
 */
function _build_client_cms_menu_links() {

  $tree = menu_tree_all_data('client-cms-menu');
  $second_level_tree = array();
  $menu['first_menu'] = array();

  //CMS Home Page.
  $menu['first_menu']['admin/client_cms'] = t('Home');

  // First level of menu
  foreach ($tree as $first_level_menu) {
    if ($first_level_menu['link']['hidden']) {
      continue;
    }

    if ($first_level_menu['link']['href'] == '<nolink>') {
      $menu_path = $first_level_menu['link']['href'] . '-' . $first_level_menu['link']['weight'];
    }
    else {
      $menu_path = $first_level_menu['link']['href'];
    }
    $menu['first_menu'][$menu_path] = t($first_level_menu['link']['link_title']);

    if (!empty($first_level_menu['below'])) {
      $new_below = array();
      foreach ($first_level_menu['below'] as $below) {
        if (!$below['link']['hidden']) {
          $new_below[] = $below;
        }
      }
      $first_level_menu['below'] = $new_below;
    }
    if($first_level_menu['below']){
      $second_level_tree[$menu_path] = $first_level_menu['below'];
    }
  }

  // _generate_new_menu_tree($tree)

  //Second level of menu
  $third_level_tree = array();
  $menu['second_menu'] = array();
  foreach ($second_level_tree as $second_level_path => $second_level_menu) {
    foreach ($second_level_menu as $second_level_item) {
      if ($second_level_item['link']['hidden']) {
        continue;
      }

      $menu_path = $second_level_item['link']['href'];

      if ($second_level_item['link']['href'] == '<nolink>') {
        $menu_path = $second_level_item['link']['href'] . '-' . $second_level_item['link']['weight'];
      }

      $menu['second_menu'][$second_level_path][$menu_path] = t($second_level_item['link']['link_title']);

      if (!empty($second_level_item['below'])) {
        $new_below = array();
        foreach ($second_level_item['below'] as $below) {
          if (!$below['link']['hidden']) {
            $new_below[] = $below;
          }
        }
        $second_level_item['below'] = $new_below;
      }
      if($second_level_item['below']){
        $third_level_tree[$menu_path] = $second_level_item['below'];
      }
    }
  }

  //Third level of menu
  $menu['third_menu'] = array();
  foreach ($third_level_tree as $third_level_path => $third_level_menu) {
    foreach ($third_level_menu as $third_level_item) {
      if (!$third_level_item['link']['hidden']) {
        $menu['third_menu'][$third_level_path][$third_level_item['link']['href']] = t($third_level_item['link']['link_title']);
        $third_level_tree[$menu_path] = $second_level_item['below'];
      }
    }
  }

  return $menu;
}

/**
 * Builds the menu from the links generated from _build_spring_cms_menu_links().
 *
 */
function _build_client_cms_menu() {
  $menu = _build_client_cms_menu_links();
  $active_parents = _build_client_cms_menu_active_parents($menu);
  $output = '<div id = "first-menu"><div class = "first-menu-inner">';
  $i = 0;
  $ids = array();
  foreach ($menu['first_menu'] as $path => $title) {
    $last_arg = ++$i == count($menu['first_menu']) ? ' last' : '';
    $last_arg .= $active_parents['first_menu'] == $path ? ' active' : '';
    $ids[$path] = $i;
    if (strpos($path, '<nolink>') !== FALSE) {
      $path = '<nolink>';
    }
    $output .= l($title, $path, array('attributes' => array('class' => array($last_arg), 'rel' => 'menu-id-' . $i)));
  }
  $output .= '</div></div><div id = "second-menu-wrap">';
  $output .= '<div id = "second-menu" class = "ddsmoothmenu">';

  foreach ($menu['second_menu'] as $parent_path => $second_menus) {
    $i = 0;
    $style = ($active_parents['second_menu'] == $parent_path) ? ' class = "active"' : '';
    $output .= '<ul id = "menu-id-' . $ids[$parent_path] . '"' . $style . '>';
    foreach ($second_menus as $path => $title) {
      if (strpos($path, '<nolink>') !== FALSE) {
        $path = '<nolink>';
      }
      $output .= '<li ' . ($i++ == 0 ? 'class="first"' : '') . ' >' . l($title, $path, array('attributes' => array('class' => array($active_parents['second_menu'] == $path ? ' active' : ''))));
      // If there is available third level menu, then add it for enabling throught jQuery as dropdown
      if (isset($menu['third_menu'][$path])) {
        $output .= '<ul>';
        foreach ($menu['third_menu'][$path] as $t_path => $t_title) {
          $output .= '<li>' . l($t_title, $t_path, array('attributes' => array('class' => array($active_parents['third_menu'] == $t_path ? 'active' : '')))) . '</li>';
        }
        $output .= '</ul>';
      }
      $output .= '</li>';
    }
    $output .= '</ul>';
  }
  $output .= '</div></div>';
  return $output;
}

/**
 * Builds the menu from the links generated from _build_spring_cms_menu_links() and factors in color variation.
 *
 */
function _build_client_cms_menu_with_color() {

  $menu = _build_client_cms_menu_links();
  $active_parents = _build_client_cms_menu_active_parents($menu);
  $output = '<div id = "first-menu">';
  $i = 0;
  $ids = array();
  foreach ($menu['first_menu'] as $path => $title) {
    $last_arg = ++$i == count($menu['first_menu']) ? ' last' : '';
    $last_arg .= $active_parents['first_menu'] == $path ? ' active' : '';
    $ids[$path] = $i;
    $output .= l($title, $path, array('attributes' => array('class' => array($last_arg), 'style' => 'background-color:green;color:pink;', 'rel' => 'menu-id-' . $i)));
  }
  $output .= '</div><div id = "second-menu-wrap" style="background-color:purple;">';
  $output .= '<div id = "second-menu" class = "ddsmoothmenu">';
  foreach ($menu['second_menu'] as $parent_path => $second_menus) {
    $i = 0;
    $style = $active_parents['second_menu'] == $parent_path ? ' class = "active"' : '';
    $output .= '<ul id = "menu-id-' . $ids[$parent_path] . '"' . $style . '>';
    foreach ($second_menus as $path => $title) {
      $output .= '<li ' . ($i++ == 0 ? 'class="first"' : '') . ' >' . l($title, $path, array('attributes' => array('class' => array($active_parents['second_menu'] == $path ? ' active' : ''))));
      // If there is available third level menu, then add it for enabling throught jQuery as dropdown
      if (isset($menu['third_menu'][$path])) {
        $output .= '<ul>';
        foreach ($menu['third_menu'][$path] as $t_path => $t_title) {
          $output .= '<li>' . l($t_title, $t_path, array('attributes' => array('class' => array($active_parents['third_menu'] == $t_path ? 'active' : '')))) . '</li>';
        }
        $output .= '</ul>';
      }
      $output .= '</li>';
    }
    $output .= '</ul>';
  }
  $output .= '</div></div>';
  return $output;
}

/**
 * Makes the parent item in the menu active when on that path.
 *
 */
function _build_client_cms_menu_active_parents($menu) {
  $url = _client_menu_manager_url_alias();
  $active_parents = array('first_menu' => '', 'second_menu' => '', 'third_menu' => '');
  // Find parent of active third menu if it exists
  foreach ($menu['third_menu'] as $parent_path => $tabs) {
    foreach ($tabs as $path => $title) {
      if ($path == $url) {
        $active_parents['third_menu'] = $parent_path;
        break;
      }
    }
  }

  foreach ($menu['second_menu'] as $parent_path => $tabs) {
    foreach ($tabs as $path => $title) {
      if ($path == $url || $path == $active_parents['third_menu']) {
        $active_parents['second_menu'] = $parent_path;
        $active_parents['second_active'] = $path;
        break;
      }
    }
  }

  foreach ($menu['first_menu'] as $path => $title) {
    if ($path == $url || $path == $active_parents['second_menu']) {
      $active_parents['second_menu'] = $path;
      $active_parents['first_menu'] = $path;
      break;
    }
  }
  return $active_parents;
}

/**
 * Find the current path.
 *
 */
function _client_menu_manager_url_alias($url = '') {
  static $alias = NULL;
  if ($url != '') {
    $alias = $url;
  }
  else {
    // If no alias attached return the received url
    return !is_string($alias) ? current_path() : $alias;
  }
}
