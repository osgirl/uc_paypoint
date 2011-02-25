<?php

/**
 * @file
 * Administrative forms and page callbacks for UC Paypoint.
 */

/**
 * Page callback to show the paypoint payment form and some explanation.
 */
function uc_paypoint_terminal($order) {
  $output = l(t('Return to order view screen.'), 'admin/store/orders/'. $order->order_id);

  $output .= '<p>'. t('Use this terminal to process paypoint payments.') .'</p>';

  $context = array(
    'revision' => 'formatted-original',
    'type' => 'order_total',
    'subject' => array(
      'order' => $order,
    ),
  );
  
  $output .= '<div><strong>'. t('Order total: @total', array('@total' => uc_price($order->order_total, $context))) .'</strong></div>';
  $output .= '<div><strong>'. t('Payment balance: @balance', array('@balance' => uc_price(uc_payment_balance($order), $context))) .'</strong></div>';

  $output .= drupal_get_form('uc_paypoint_terminal_form', $order);
  return $output;
}

/**
 * Administrative payment terminak.
 *
 * It simply adds an amount field and unsets the amount specified in the paypoint data array.
 * Redirect after submission is handled the same way as standard PayPoint payments.
 */
function uc_paypoint_terminal_form($form_state, $order) {
  $form['paypoint_card'] = array(
    '#type' => 'fieldset',
    '#title' => t('Credit card details'),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
  );
  $form['paypoint_card'] += uc_paypoint_credit_form(); // Use the same credit card form as a basis.

  $balance = uc_payment_balance($order);

  $context = array(
    'revision' => 'formatted-original',
    'type' => 'amount',
  );

  $options = array(
    'sign' => FALSE,
    'thou' => FALSE,
    'dec' => '.',
  );

  $amount = $balance > 0 ? uc_price($balance, $context, $options) : 0;
  $prefix = variable_get('uc_sign_before_amount', FALSE) ? '' : variable_get('uc_currency_sign', '$');
  $suffix = variable_get('uc_sign_after_amount', FALSE) ? variable_get('uc_currency_sign', '$') : '';

  // Let the administrator set the amount to charge.
  $form['amount_explanation'] = array(
    '#type' => 'markup',
    '#value' => t('Amount to be charged: @amount <br />This amount is fixed and equals to the total order value.', array('@amount' => $prefix . $amount . $suffix)),
    '#prefix' => '<div>',
    '#suffix' => '</div>',
  );
  
  $form['order'] = array(
    '#type' => 'value',
    '#value' => $order,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit order'),
  );
  return $form;
}

/**
 * Validation handler for the terminal form.
 */
function uc_paypoint_terminal_form_validate($form, &$form_state) {
  $card_data = $form_state['values']['paypoint_card'];
  // Make sure an owner value was entered.
  if (empty($card_data['cc_owner'])) {
    form_set_error('cc_owner', t('Enter the owner name as it appears on the card.'));
  }

  // Validate the CC number if that's turned on/check for non-digits.
  if (!_valid_card_number($card_data['cc_number']) || !ctype_digit($card_data['cc_number'])) {
    form_set_error('cc_number', t('You have entered an invalid credit card number.'));
  }

  // Validate the start date (if entered).
  if (!_valid_card_start($card_data['cc_start_month'], $card_data['cc_start_year'])) {
    form_set_error('cc_start_month', t('The start date you entered is invalid.'));
    form_set_error('cc_start_year');
  }

  // Validate the card expiration date.
  if (!_valid_card_expiration($card_data['cc_exp_month'], $card_data['cc_exp_year'])) {
    form_set_error('cc_exp_month', t('The credit card you entered has expired.'));
    form_set_error('cc_exp_year');
  }

  // Validate the issue number (if entered).  With issue numbers, '01' is
  // different from '1', but is_numeric() is still appropriate.
  if (!_valid_card_issue($card_data['cc_issue'])) {
    form_set_error('cc_issue', t('The issue number you entered is invalid.'));
  }

  // Validate the CVV number if enabled.
  if (!_valid_cvv($card_data['cc_cvv'])) {
    form_set_error('cc_cvv', t('You have entered an invalid CVV number.'));
  }
}

/**
 * Submit handler for the terminal form.
 */
function uc_paypoint_terminal_form_submit($form, &$form_state) {
  $order = $form_state['values']['order'];
  $order->payment_details = $form_state['values']['paypoint_card'];
  $results = uc_paypoint_xmlrpc_request($order);
  uc_paypoint_complete_request($results);
  $form_state['redirect'] = 'admin/store/orders/'. $order->order_id;
}