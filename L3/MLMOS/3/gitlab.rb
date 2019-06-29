external_url 'https://gitlab.example.com'
registry_external_url 'https://gitlab.example.com:4567'

gitlab_rails['initial_root_password'] = File.read('/run/secrets/gitlab_root_password')
