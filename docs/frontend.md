# Frontend Development

To take part in the development of the frontend application you need to install
[nodejs](https://nodejs.org/en/) with [yarn](https://yarnpkg.com/en/) package manager.

## OpenAPI rendering

```bash
docker run -p 80:8080 -e SWAGGER_JSON=/docs/openapi.yml -v (pwd):/docs swaggerapi/swagger-ui
```

```bash
npx redoc-cli serve --watch docs/openapi.yml
```

## Generate API docs

Generate API documentation in HTML format

```
npx redoc-cli bundle -o docs/dist/index.html docs/openapi.yml
```

## Mock server

```
prism mock docs/openapi.yml
```