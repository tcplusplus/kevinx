module.exports = {
  root: true,
  env: {
    node: true,
    mocha: true
  },
  plugins: [],
  extends: [
    'plugin:vue/recommended',
    '@vue/typescript',
    'plugin:@typescript-eslint/eslint-recommended',
    // "plugin:@typescript-eslint/recommended",
    // "plugin:@typescript-eslint/recommended-requiring-type-checking"
  ],
  rules: {
    'no-console': process.env.NODE_ENV === 'production' ? 'off' : 'off',
    'no-debugger': process.env.NODE_ENV === 'production' ? 'error' : 'off',
    '@typescript-eslint/consistent-type-assertions': ['error', { 
      assertionStyle: 'as'
    }],
    'no-var': 'error',
    'no-alert': 'error',
    'new-cap': ['error', { 'newIsCapExceptions': ['myPeerConnection', 'CefTimestamp'] }],
    'vue/html-closing-bracket-newline': 'error',
    'vue/html-closing-bracket-spacing': 'error',
    'vue/html-end-tags': 'error',
    'vue/html-indent': 'error',
    'vue/html-quotes': 'error',
    'vue/html-self-closing': 'error',
    'vue/order-in-components': 'error',
    'vue/no-use-v-if-with-v-for': 'error',
    // "@typescript-eslint/no-untyped-public-signature": ["error", { "ignoredMethods": ["ignoredMethodName"] }],
    /* 
    'vue/html-self-closing': ['error', {
      "html": {
        "void": "never",
        "normal": "always",
        "component": "always"
      },
      "svg": "always",
      "math": "always"
    }],*/
    'vue/max-attributes-per-line': ['error', {
      'singleline': 3,
      'multiline': {
        'max': 1,
        'allowFirstLine': false
      }
    }],
    'vue/no-multi-spaces': ['error', {
      'ignoreProperties': false
    }],
    'vue/mustache-interpolation-spacing': 'error',
    // 'vue/require-default-prop': 'error',
    'vue/require-prop-types': 'error',
    'vue/no-unused-components': 'error',
    'prefer-const': 'error',
    'no-unreachable': 'error',
    'no-eq-null': 'error',
    'dot-notation': ['error', {            // http://eslint.org/docs/rules/dot-notation
      'allowKeywords': true
    }],
    'no-new': 'error',                     // http://eslint.org/docs/rules/no-new
    'no-new-func': 'error',                // http://eslint.org/docs/rules/no-new-func
    'no-param-reassign': 'error',           // http://eslint.org/docs/rules/no-param-reassign
    'complexity': ['error', { 'max': 12 }],
    'max-depth': ['error', 3],
    'max-len': ['error', { 'code': 140 }],
    'max-lines': ['error', 300],            // goal is 200
    'max-lines-per-function': ['error', 35],
    'vue/component-name-in-template-casing': ['error', 'PascalCase'],
    'semi': ['error', 'never'],
    'quotes': ['error', 'single']
    // 'no-shadow': 'error'
    // 'promise/avoid-new': 'error' // https://github.com/xjamundx/eslint-plugin-promise/issues/111
    // 'promise/always-return': 'error'
    // Check next rules: https://vuejs.github.io/eslint-plugin-vue/rules/
    // other possible rules
    // ,
    // "promise/no-native": "off",
    // "promise/no-nesting": "warn",
    // "promise/no-promise-in-callback": "warn",
    // "promise/no-callback-in-promise": "warn",
    // "promise/no-new-statics": "error",
    // "promise/no-return-in-finally": "warn",
    // "promise/valid-params": "warn"    "no-console": process.env.NODE_ENV === "production" ? "error" : "off",
  },
  parserOptions: {
    'parser': '@typescript-eslint/parser',
    'ecmaVersion': 2017,
    'sourceType': 'module'
  },
  overrides: [
    {
      files: [
        '**/__tests__/*.{j,t}s?(x)',
        '**/tests/unit/**/*.spec.{j,t}s?(x)'
      ],
      env: {
        jest: true
      }
    }
  ]
}
