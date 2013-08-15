<?php

function _openstates_search() {
  $content = '';
  $content .= '<h2>Open States</h2>';
  $apikey = variable_get('openstates_key', '');
  if (empty($apikey)) {

    drupal_set_message('No API key found.', 'error');

  } else
  {

    $content .= 'Your API key is: ' . $apikey;

    $q = "tobacco";
    $updated_since = '2013-03-01';

    #$url = 'http://openstates.org/api/v1/bills/?q=agriculture&state=vt&chamber=upper&apikey=YOUR_API_KEY';
    // $url = 'http://openstates.org/api/v1/bills/?q=' . $q . '&updated_since=' . $updated_since . '&apikey=' . $apikey;

    #print_r($url); exit;

    // echo '<pre>'; var_dump ($data); echo '</pre>';
    //

    // echo '<pre>'; var_dump ($bills); echo '</pre>';

    $searchform = drupal_get_form('openstates_search_form');
    dpm($searchform, 'form state');
    $content .= drupal_render($searchform);


    /*
    $i = 0;

    foreach ($bills AS $a => $b) {

      $i++;
      // echo '<pre>'; print_r($bill); echo '</pre><hr />';
      $content .= '
      <p>
      ' . $i . ') <br />
      <strong>State:</strong> ' . strtoupper($b["state"]);
      echo '<br /><strong>Title:</strong> ' . $b["title"];
      echo '<br /><strong>Bill ID:</strong> ';
      echo '<a href="bill.php?state=' . $b["state"] . '&bill_id=' . $b["bill_id"] . '&session=' . $b["session"] . '&chamber=' . $b["chamber"] . '">';
      echo $b["bill_id"];
      echo '</a>';
      if (!empty($b["subjects"])) echo '<br /><strong>Subjects:</strong> ' . implode(', ', $b["subjects"]);
      echo '<br /><strong>Updated:</strong> ' . $b["updated_at"];
      echo '</p>';

    }
 */

  }

  return $content;

}

function openstates_search_form($form, &$form_state) {


  $form['search'] = array(
    '#input' => TRUE,
    '#type' => 'textfield',
    '#size' => '24',
    '#title' => t('Search term'),
    // Use theme('textfield') to format this element on output.
    '#theme' => array('textfield'),

    // Do not provide autocomplete.
    '#autocomplete_path' => FALSE,

    // Allow theme('form_element') to control the markup surrounding this
    // value on output.
    '#theme_wrappers' => array('form_element'),
    '#default_value' => !empty($form_state["values"]["search"]) ? $form_state["values"]["search"] : '',
  );
  $form['updated_since'] = array(
    '#type' => 'hidden',
    '#value' => '2013-03-01',
    '#description' => 'Updated since: 2013-03-01',
  );
  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  return $form;

}

function openstates_search_form_validate($form, &$form_state) {
}

function openstates_search_form_submit($form, &$form_state) {

  // dpm($form_state);
  $results = openstates_apicall($form, $form_state);

  // dpm($results, "results");

  // dpm($bills, "BILLS");
  $header = array(
    array('data' => 'Bill ID', 'field' => 'bill_id'),
    array('data' => 'Title', 'field' => 'title'),
    array('data' => 'State', 'field' => 'state'),
  );

  $content = openstates_print($header, $results);
  return $content;
/*
  $content = '';
    $content .= theme("table",
      array (
        'header' => $header,
        'rows' => $results,
        )
      );
   */

  $content = theme("table",
      array (
        'header' => $header,
        'rows' => $results,
        )
    );

  render($content);

  // dpm($content, "CONTENT");

}

function openstates_apicall($form, &$form_state) {

  $url = 'http://openstates.org/api/v1/bills/?q=' . $form_state["values"]["search"] . '&updated_since=' . $form_state["values"]["updated_since"] . '&apikey=' . variable_get('openstates_key', '');

  // print_r($url); exit;
  dpm($url, 'URL');

  $json = file_get_contents($url,0,null,null);
  $json_output = json_decode($json, true);

  $request = drupal_http_request($url);
  $data = drupal_json_decode($request->data);

    // $content .= '
    // <p><strong>Query:</strong> ' . $q . '
    // <br /><strong>Results</strong> ' . count($data) . '
    // </p>';
    $bills = array();
    foreach ($data AS $a => $bill) {
      // $bills{$a} = array("title" => $bill["title"]);
      $bills{$a} = array(
        'data' => array("id"=>$bill['bill_id'], "title"=>$bill['title'],"state"=>$bill['state']),
      );
    }

    return $bills;


}

function openstates_print($header, $data) {

  // dpm($header, "HEADER");
  // dpm($data, 'DATA');
  $content = '<hr />';
$content .= theme("table",
      array (
        'header' => $header,
        'rows' => $data,
        )
    );

  // dpm($content);
  return $content;

}
