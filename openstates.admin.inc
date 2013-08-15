<?php

/**
 * @file
 * Administrative page callbacks for the googleanalytics module.
 */

/**
 * Implements hook_admin_settings() for module settings configuration.
 */
function openstates_admin_settings_form($form_state) {
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );

  $form['account']['openstates_key'] = array(
    '#title' => t('API Key'),
    '#type' => 'textfield',
    '#default_value' => variable_get('openstates_key', ''),
    '#size' => 30,
    '#maxlength' => 40,
    '#required' => TRUE,
    '#description' => t('The Open States provides a RESTful API for accessing state legislative information. The required key can be obtained from <a href="@openstates">Sunlight Labs</a>.', array('@openstates' => 'http://services.sunlightlabs.com/')),
  );

  return system_settings_form($form);

}


/**
 * Implements _form_validate().
 */
