<?php

function _get_recent_page_updates() {
  $basic_page_items = array();
  $table = array();
  $rows = array();
  $headers = array(t('Page Name'), t('Last Updated'), t('Operations'));

 /**
 * The query to get the data for what we want to display in this block
 */
  $query = db_select('node', 'n');
  $query->fields('n', array('nid'))
  ->fields('n', array('nid'))
  ->condition('n.type', 'basic_page')
  ->orderBy('n.changed', 'DESC')
  ->range(0,5);
  $result = $query->execute()->fetchAllKeyed();
  $basic_page_nids = array_keys($result);

  if (is_array($basic_page_nids) && !empty($basic_page_nids)) {
    $basic_page_items = node_load_multiple($basic_page_nids);
  }

  if (!empty($basic_page_items)) {
    foreach ($basic_page_items AS $key => $basic_page_node) {
        // dpm($basic_page_node);
        $rows[$key][] = array('data' => t($basic_page_node->title));
        $rows[$key][] = array('data' => date('d/m/Y', $basic_page_node->changed));
        $rows[$key][] = array('data' => l('edit', 'node/'.$basic_page_node->nid. '/edit'));
    }
  }


  $rows[(count($rows) + 2)][] = array('data' => l('View All Website Pages', 'admin/client_cms/manage-basic-pages'), 'colspan' => 3);

  $table = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $headers,
    );

  return $table;
}
