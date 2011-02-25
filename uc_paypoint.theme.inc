<?php
/**
 * @file
 * Theem functions for UC Paypoint.
 */

/**
 * Theme the list of cards PayPoint accepts.
 */
function theme_uc_paypoint_cards() {
  $image_path = drupal_get_path('module', 'uc_paypoint') . '/images/';

  $card_types = array(
    'visa' => 'Visa',
    'delta' => 'Delta',
    'electron' => 'Electron',
    'mastercard' => 'Mastercard',
    'maestro' => 'Maestro',
    'solo' => 'Solo',
    'amex' => 'AMEX',
    'jbc' => 'JBC',
    'laser' => 'Laser',
  );
  
  foreach ($card_types as $card => $title) {
    $cards[] = theme('image', $image_path . $card . '.gif', '', '', array('style' => 'position:relative; top:5px; margin-right:4px;'));
  }
  
  // TODO use CSS and theme('item_list')
  return '<span>' . implode('', $cards) . '</span>';
}

function theme_uc_paypoint_logo() {
  $image_path = drupal_get_path('module', 'uc_paypoint') . '/images/powered-by-paypoint.png';
  $title = variable_get('uc_paypoint_title', t('PayPoint'));
  return theme('image', $image_path, $title, $title, array('style' => 'position:relative; top:6px;'));
}


// Themes the form to be in a compact table.
function theme_uc_paypoint_credit_form($form) {
  // Comment out this function to just straight display the form.
  $form['cc_number']['#title'] = '';
  $form['cc_start_month']['#title'] = '';
  $form['cc_start_year']['#title'] = '';
  $form['cc_exp_month']['#title'] = '';
  $form['cc_exp_year']['#title'] = '';
  $form['cc_issue']['#title'] = '';

  if (arg(1) == 'checkout') {
    $path = base_path() . drupal_get_path('module', 'uc_credit');
    $output = '<table class="inline-pane-table" cellpadding="2">';
    if (strlen($form['cc_policy']['#value']) > 0) {
      $output .= '<tr><td colspan="2">'. $form['cc_policy']['#value']
                .'</td></tr>';
    }
    if (variable_get('uc_credit_type_enabled', FALSE)) {
      $form['cc_type']['#title'] = '';
      $output .= '<tr><td class="field-label">'. t('Card Type:') .'</td><td>'
               . drupal_render($form['cc_type']) .'</td></tr>';
    }
    if (variable_get('uc_credit_owner_enabled', FALSE)) {
      $form['cc_owner']['#title'] = '';
      $output .= '<tr><td class="field-label">'. t('Card Owner:') .'</td><td>'
               . drupal_render($form['cc_owner']) .'</td></tr>';
    }
    $output .= '<tr><td class="field-label">'. t('Card Number:') .'</td><td>'
             . drupal_render($form['cc_number']) .'</td></tr>';
    if (variable_get('uc_credit_start_enabled', FALSE)) {
      $output .= '<tr><td class="field-label">'. t('Start Date:') .'</td><td>'
               . drupal_render($form['cc_start_month']) .' '. drupal_render($form['cc_start_year'])
                .' '. t('(if present)') .'</td></tr>';
    }
    $output .= '<tr><td class="field-label">'. t('Expiration Date:') .'</td><td>'
             . drupal_render($form['cc_exp_month']) .' '
             . drupal_render($form['cc_exp_year']) .'</td></tr>';
    if (variable_get('uc_credit_issue_enabled', FALSE)) {
      $output .= '<tr><td class="field-label">'. t('Issue Number:') .'</td><td>'
               . drupal_render($form['cc_issue']) .' '. t('(if present)') .'</td></tr>';
    }
    if (variable_get('uc_credit_cvv_enabled', TRUE)) {
      $form['cc_cvv']['#title'] = '';
      $output .= '<tr><td class="field-label">'. t('CVV:') .'</td><td>'. drupal_render($form['cc_cvv'])
                .' <img src="'. $path .'/images/info.png" onclick="cvv_info_popup();" style="cursor: pointer; position: relative; top: 3px;"> <a style="cursor: pointer; font-weight: normal;" onclick="cvv_info_popup();">'
                . t("What's the CVV?") .'</a></td></tr>';
    }
    if (variable_get('uc_credit_bank_enabled', FALSE)) {
      $form['cc_bank']['#title'] = '';
      $output .= '<tr><td class="field-label">'. t('Issuing Bank:') .'</td><td>'
               . drupal_render($form['cc_bank']) .'</td></tr>';
    }
    $output .= '</table>';
  }
  else {
    $output = '<table class="order-edit-table"><tbody style="border-top: 0px;">';
    if (variable_get('uc_credit_type_enabled', FALSE)) {
      $form['cc_type']['#title'] = '';
      $output .= '<tr><td class="oet-label">'. t('Card Type:') .'</td><td>'
               . drupal_render($form['cc_type']) .'</td></tr>';
    }
    if (variable_get('uc_credit_owner_enabled', FALSE)) {
      $form['cc_owner']['#title'] = '';
      $output .= '<tr><td class="oet-label">'. t('Card Owner:') .'</td><td>'
               . drupal_render($form['cc_owner']) .'</td></tr>';
    }
    $output .= '<tr><td class="oet-label">'. t('Card Number:') .'</td><td>'
             . drupal_render($form['cc_number']) .'</td></tr>';
    if (variable_get('uc_credit_start_enabled', FALSE)) {
      $output .= '<tr><td class="oet-label">'. t('Start Date:') .'</td><td>'
               . drupal_render($form['cc_start_month']) .' '. drupal_render($form['cc_start_year'])
                .' '. t('(if present)') .'</td></tr>';
    }
    $output .= '<tr><td class="oet-label">'. t('Expiration Date:') .'</td><td>'
             . drupal_render($form['cc_exp_month']) .' '
             . drupal_render($form['cc_exp_year']) .'</td></tr>';
    if (variable_get('uc_credit_issue_enabled', FALSE)) {
      $output .= '<tr><td class="oet-label">'. t('Issue Number:') .'</td><td>'
               . drupal_render($form['cc_issue']) .' '. t('(if present)') .'</td></tr>';
    }
    if (variable_get('uc_credit_cvv_enabled', TRUE)) {
      $form['cc_cvv']['#title'] = '';
      $output .= '<tr><td class="oet-label">'. t('CVV:') .'</td><td>'
               . drupal_render($form['cc_cvv']) .'</td></tr>';
    }
    if (variable_get('uc_credit_bank_enabled', FALSE)) {
      $form['cc_bank']['#title'] = '';
      $output .= '<tr><td class="oet-label">'. t('Issuing Bank:') .'</td><td>'
               . drupal_render($form['cc_bank']) .'</td></tr>';
    }
    $output .= '</td></tr></tbody></table>';
  }

  return $output;
}
