function toggleCollapsed(linkObj, text)
{
  var base = $(linkObj).attr('id');
  var content = $('#' + base + '-content');
  var header  = $('#' + base + '-header');

  toggleVisibility(linkObj);
  if (content.is(':visible') === true) {
    $(header).text('- ' + text);
  }
  else {
    $(header).text('+ ' + text);
  }
}
