# Frontend Development

To take part in the development of the frontend application you need to install
[nodejs](https://nodejs.org/en/) with [yarn](https://yarnpkg.com/en/) package manager.

## OpenAPI rendering

```bash
docker run -p 80:8080 -e SWAGGER_JSON=/docs/openapi.yml -v (pwd):/docs swaggerapi/swagger-ui
```