# Install puppet vesrsion 2.5.0

package { 'puppet-lint':
  ensure   => '2.5.2',
  provider => 'gem',
  }
