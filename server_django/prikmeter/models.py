from django.db import models


class AuthToken(models.Model):
    token = models.CharField(primary_key=True, max_length=64)
    owner = models.ForeignKey('auth.User', models.DO_NOTHING, blank=True, null=True)


class Meter(models.Model):
    id = models.AutoField(primary_key=True)
    identification_string = models.CharField(max_length=255)
    type = models.IntegerField(choices=(
        (1, 'electricity'),
        (2, 'gas'),
    ))
    owner = models.ForeignKey('auth.User', models.DO_NOTHING, blank=True, null=True)


class Telegram(models.Model):
    id = models.AutoField(primary_key=True)
    owner = models.ForeignKey('auth.User', models.DO_NOTHING, blank=True, null=True)
    telegram = models.BinaryField(blank=True, null=True)
    upload_timestamp = models.DateTimeField(blank=True, null=True)


class ElectricityReading(models.Model):
    id = models.AutoField(primary_key=True)
    meter = models.ForeignKey('Meter', models.DO_NOTHING, blank=True, null=True)
    timestamp = models.DateTimeField()
    total_consumption_kwh_low = models.FloatField(blank=True, null=True)
    total_consumption_kwh_high = models.FloatField(blank=True, null=True)
    total_production_kwh_low = models.FloatField(blank=True, null=True)
    total_production_kwh_high = models.FloatField(blank=True, null=True)
    current_consumption_kw = models.FloatField(blank=True, null=True)
    current_production_kw = models.FloatField(blank=True, null=True)


class GasReading(models.Model):
    id = models.AutoField(primary_key=True)
    meter = models.ForeignKey('Meter', models.DO_NOTHING, blank=True, null=True)
    timestamp = models.DateTimeField()
    total_consumption_m3 = models.FloatField(blank=True, null=True)


class Setting(models.Model):
    key = models.CharField(primary_key=True, max_length=255)
    value = models.CharField(max_length=255, blank=True, null=True)
