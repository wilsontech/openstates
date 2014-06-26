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
    //dpm($searchform, 'form state');
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
    // Use theme('textfield') to format thias element on output.
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
 $form['state'] = array(
    '#type' => 'select',
    '#title' => t('Selected'),
    '#options' => array(
      0 => t('select state'),
     'AL' => t('Alabama'),
'AK' => t('Alaska'),
'AZ' => t('Arizona'),
'CA' => t('California'),
'CO' => t('Colorado'),
'CT' => t('Connecticut'),
'DE' => t('Delaware'),
'FL' => t('Florida'),
'GA' => t('Georgia'),
'HI' => t('Hawaii'),
'ID' => t('Idaho'),
'IL' => t('Illinois'),
'IN' => t('Indiana'),
'IA' => t('Iowa'),
'KS' => t('Kansas'),
'KY' => t('Kentucky[C]'),
'LA' => t('Louisiana'),
'ME' => t('Maine'),
'MD' => t('Maryland'),
'MA' => t('Massachusetts[D]'),
'MI' => t('Michigan'),
'MN' => t('Minnesota'),
'MS' => t('Mississippi'),
'MO' => t('Missouri'),
'MT' => t('Montana'),
'NE' => t('Nebraska'),
'NV' => t('Nevada'),
'NH' => t('New Hampshire'),
'NJ' => t('New Jersey'),
'NM' => t('New Mexico'),
'NY' => t('New York'),
'NC' => t('North Carolina'),
'ND' => t('North Dakota'),
'OH' => t('Ohio'),
'OK' => t('Oklahoma'),
'OR' => t('Oregon'),
'PA' => t('Pennsylvania[E]'),
'RI' => t('Rhode Island[F]'),
'SC' => t('South Carolina'),
'SD' => t('South Dakota'),
'TN' => t('Tennessee'),
'TX' => t('Texas'),
'UT' => t('Utah'),
'VT' => t('Vermont'),
'VA' => t('Virginia[G]'),
'WA' => t('Washington'),
'WV' => t('West Virginia'),
'WI' => t('Wisconsin'),
'WY' => t(' Wyoming'),


    ),
    //'#default_value' => $category['selected'],
    '#description' => t('Set this to <em>Yes</em> if you would like this category to be selected by default.'),
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );
  
  // dpm($form_state);
  $results = openstates_apicall($form, $form_state);
//print_r($results); die();

  // dpm($bills, "BILLS");
  $header = array(
    array('data' => 'Bill ID', 'field' => 'bill_id'),
    array('data' => 'Title', 'field' => 'title'),
    array('data' => 'State', 'field' => 'state'),
    array('data' => 'Date', 'field' => 'created_at'),
  );

  $content = openstates_print($header, $results);
 
/*


  $content = '';
    $content .= theme("table",
      array (
        'header' => $header,
        'rows' => $results,
        )
      );
   */

/*
  $content = theme("table",
      array (
        'header' => $header,
        'rows' => $results,
        )
    );
  dpm($content, "CONTENT");

  render($content);
*/
  $form['markup'] = array(
     '#markup' => $content
  );
  return $form;

}

function openstates_search_form_validate($form, &$form_state) {
}

function openstates_search_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

function openstates_apicall($form, &$form_state) {
//echo "<pre>";print_r($form_state); die();

  $url = 'http://openstates.org/api/v1/bills/?q=' . @$form_state["values"]["search"] . '&state=' . @$form_state["values"]["state"] . '&updated_since=' . @$form_state["values"]["updated_since"] . '&apikey=' . variable_get('openstates_key', '');

   ///print_r($url); exit;
  // dpm($url, 'URL');

 // $json = file_get_contents($url,0,null,null); 
 // $json_output = json_decode($json, true);

  $request = drupal_http_request($url);
  $data = drupal_json_decode($request->data);
//echo "<pre>"; print_r($data); die();
    // $content .= '
    // <p><strong>Query:</strong> ' . $q . '
    // <br /><strong>Results</strong> ' . count($data) . '
    // </p>';
    $bills = array();
if(isset($data))
{
    foreach ($data AS $a => $bill) {
//echo "<pre>"; print_r($bill);die();
      // $bills{$a} = array("title" => $bill["title"]);
      $bills{$a} = array(
        'data' => array("id"=>$bill['bill_id'], "title"=>$bill['title'],"state"=>$bill['state'],'date'=>$bill['created_at']),
      );
    }
}

    return $bills;



}

function openstates_print($header, $data) {
 // echo '<hr />'; exit;
  // dpm($header, "HEADER");
  // dpm($data, 'DATA');



  $content = '<hr />';
  $content .= theme("table",
      array (
        'header' => $header,
        'rows' => $data,
        )
    );

  //$content = $data;
  return $content;

}
