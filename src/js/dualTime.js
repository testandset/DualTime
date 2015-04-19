/*
 * Pebble app ready
 */
Pebble.addEventListener('ready', 
  function(e) {
    console.log("DualTime ready!");
  }
);

/*
 * Web-view-closed event handler
 * Get all the data submitted from config and pass it to app. This method also stores
 * config data in loacal storage.
 */
Pebble.addEventListener('webviewclosed',
  function(e) {
    var options = JSON.parse(decodeURIComponent(e.response)),
         // -1 as getTimezoneOffset return negative or timezones ahead of UTC
        currentOffset = new Date().getTimezoneOffset() * 60 * -1; 
    
    // Only send to Pebble if we have valid data
    if(options.offset !== null && options.offset !== undefined){
      Pebble.sendAppMessage( {'0' : parseInt(currentOffset, 10), '1' : parseInt(options.offset, 10)},
              function(e) {
                console.log('Successfully delivered message with transactionId='+ e.data.transactionId);
                // If data is successfully delivered to pebble store data locally
                localStorage.setItem('location', options.location);
                localStorage.setItem('timezoneid', options.timezoneid);
                localStorage.setItem('timezonename',  options.timezonename);
              },
              function(e) {
                console.log('Unable to deliver message with transactionId=' + e.data.transactionId + ' Error is: ' + e.error.message);
              }
      );
    }
  });

/*
 * Open configration page.
 * This method also passed current local storted data to config page.
 */
Pebble.addEventListener('showConfiguration', function(e) {
  
  // Show config page
  var location = localStorage.getItem('location'),
      queryParams = "";
  
  if(location !== null){
    queryParams = "?location=" + encodeURIComponent(location) +
      "&timezoneid=" + encodeURIComponent(localStorage.getItem('timezoneid')) +
      "&timezonename=" + encodeURIComponent(localStorage.getItem('timezonename'));
  }  
  
  Pebble.openURL('https://www.googledrive.com/host/0B0tePgr0JgqOOE5OTlNlNU03Tjg' + queryParams);
});
